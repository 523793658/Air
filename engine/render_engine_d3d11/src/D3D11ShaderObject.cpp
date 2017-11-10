
#include <sstream>
#include "Context.h"
#include "SingletonManager.hpp"
#include "d3dcompiler.h"
#include <boost/functional/hash.hpp>
#include "core/include/ResLoader.h"
#include "rendersystem/include/RenderEffect.hpp"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/RenderEnvironment.hpp"
#include "render_engine_d3d11/include/D3D11RenderEngine.hpp"
#include "render_engine_d3d11/include/D3D11Texture.hpp"
#include "render_engine_d3d11/include/D3D11GraphicsBuffer.hpp"
#include "D3D11RenderStateObject.hpp"
#include "D3D11Mapping.hpp"

#include "render_engine_d3d11/include/D3D11ShaderObject.hpp"



namespace
{
	using namespace Air;
	class SetD3D11ShaderParameterTextureSRV
	{
	public:
		SetD3D11ShaderParameterTextureSRV(std::tuple<void*, uint32_t, uint32_t>& srvsrc, ID3D11ShaderResourceView*& srv, RenderEffectParameter* param): mSrvsrc(&srvsrc), mSrv(&srv), mParam(param)
		{

		}
		void operator()()
		{
			TextureSubresource tex_subres;
			mParam->getValue(tex_subres);
			if (tex_subres.mTex)
			{
				*mSrvsrc = std::make_tuple(tex_subres.mTex.get(), tex_subres.mFirstArrayIndex * tex_subres.mTex->getNumMipMaps() + tex_subres.mFirstLevel, tex_subres.mNumItems * tex_subres.mNumLevels);
				*mSrv = checked_cast<D3D11Texture*>(tex_subres.mTex.get())->retriveD3DShaderResourceView(tex_subres.mFirstArrayIndex, tex_subres.mNumItems, tex_subres.mFirstLevel, tex_subres.mNumLevels).get();
			}
			else
			{
				std::get<0>(*mSrvsrc) = nullptr;
			}
		}
	private:
		std::tuple<void*, uint32_t, uint32_t>* mSrvsrc;
		ID3D11ShaderResourceView** mSrv;
		RenderEffectParameter* mParam;
	};

	class SetD3D11ShaderParameterGraphicsBufferSRV
	{
	public:
		SetD3D11ShaderParameterGraphicsBufferSRV(std::tuple<void*, uint32_t, uint32_t>& srvsrc, ID3D11ShaderResourceView*& srv, RenderEffectParameter* param)
			: mSrvSrc(&srvsrc), mSrv(&srv), mParam(param)
		{

		}
		void operator()()
		{
			GraphicsBufferPtr buf;
			mParam->getValue(buf);
			if (buf)
			{
				*mSrvSrc = std::make_tuple(buf.get(), 0, 1);
				*mSrv = checked_cast<D3D11GraphicsBuffer*>(buf.get())->getD3DShaderResourceView().get();
			}
			else
			{
				std::get<0>(*mSrvSrc) = nullptr;
			}
		}

	private:
		std::tuple<void*, uint32_t, uint32_t>* mSrvSrc;
		ID3D11ShaderResourceView** mSrv;
		RenderEffectParameter* mParam;
	};

	class SetD3D11ShaderParameterTextureUAV
	{
	public:
		SetD3D11ShaderParameterTextureUAV(void*& uavsrc, ID3D11UnorderedAccessView*& uav, RenderEffectParameter* param): mUAVSrc(&uavsrc), mUAV(&uav), mParam(param)
		{

		}

		void operator()()
		{
			TextureSubresource tex_subres;
			mParam->getValue(tex_subres);
			if (tex_subres.mTex)
			{
				*mUAVSrc = tex_subres.mTex.get();
				*mUAV = checked_cast<D3D11Texture*>(tex_subres.mTex.get())->retriveD3DUnorderedAccessView(tex_subres.mFirstArrayIndex, tex_subres.mNumItems, tex_subres.mFirstLevel).get();
			}
			else
			{
				*mUAVSrc = nullptr;
			}
		}


	private:
		void** mUAVSrc;
		ID3D11UnorderedAccessView** mUAV;
		RenderEffectParameter* mParam;
	};


	class SetD3D11ShaderParameterGraphicsBufferUAV
	{
	public:
		SetD3D11ShaderParameterGraphicsBufferUAV(void*& uavsrc, ID3D11UnorderedAccessView*& uav, RenderEffectParameter* param)
			: mUavSrc(&uavsrc), mUAV(&uav), mParam(param)
		{

		}

		void operator()()
		{
			GraphicsBufferPtr buf;
			mParam->getValue(buf);
			if (buf)
			{
				*mUavSrc = buf.get();
				*mUAV = checked_cast<D3D11GraphicsBuffer*>(buf.get())->getD3DUnorderAccessView().get();
			}
			else
			{
				*mUavSrc = nullptr;
			}
		}

	private:
		void** mUavSrc;
		ID3D11UnorderedAccessView** mUAV;
		RenderEffectParameter* mParam;
	};


}


namespace Air
{
	D3D11ShaderObject::D3D11ShaderObject()
		:D3D11ShaderObject(MakeSharedPtr<D3D11ShaderObjectTemplate>())
	{

	}

	D3D11ShaderObject::D3D11ShaderObject(std::shared_ptr<D3D11ShaderObjectTemplate> const & so_template)
		: mSoTemplate(so_template)
	{
		mHasDiscard = true;
		mHasTessellation = false;
		mIsShaderValidate.fill(0);
	}



	bool D3D11ShaderObject::attachNativeShader(ShaderType type, RenderEffect const & effect, std::array<uint32_t, ST_NumShaderTypes> const & shader_desc_ids, std::vector<uint8_t> const & native_shader_block)
	{
		bool ret = false;
		mIsShaderValidate[type] = false;
		std::string_view shader_profile = this->getShaderProfile(type, effect, shader_desc_ids[type]);
		if (native_shader_block.size() >= 25 + shader_profile.size())
		{
			uint8_t const * nsbp = &native_shader_block[0];
			uint8_t len = *nsbp;
			++nsbp;
			std::string& profile = mSoTemplate->mShaderCode[type].second;
			profile.resize(len);
			std::memcpy(&profile[0], nsbp, len);
			nsbp += len;
			if (profile == shader_profile)
			{
				mIsShaderValidate[type] = true;
				uint32_t blob_size;
				std::memcpy(&blob_size, nsbp, sizeof(blob_size));
				blob_size = LE2Native(blob_size);
				nsbp += sizeof(blob_size);
				std::shared_ptr<std::vector<uint8_t>> code_blob = MakeSharedPtr<std::vector<uint8_t>>(blob_size);
				std::memcpy(&((*code_blob)[0]), nsbp, blob_size);
				nsbp += blob_size;



				mSoTemplate->mShaderDesc[type] = MakeSharedPtr<D3D11ShaderObjectTemplate::D3D11ShaderDesc>();
				auto & sd = *mSoTemplate->mShaderDesc[type];
				uint16_t cb_desc_size;
				std::memcpy(&cb_desc_size, nsbp, sizeof(cb_desc_size));
				nsbp += sizeof(cb_desc_size);
				cb_desc_size = LE2Native(cb_desc_size);
				sd.mCBDesc.resize(cb_desc_size);
				for (size_t i = 0; i < sd.mCBDesc.size(); ++i)
				{
					len = *nsbp;
					++nsbp;
					sd.mCBDesc[i].mName.resize(len);
					std::memcpy(&sd.mCBDesc[i].mName[0], nsbp, len);

					nsbp += len;

					sd.mCBDesc[i].mNameHash = RT_HASH(sd.mCBDesc[i].mName.c_str());

					std::memcpy(&sd.mCBDesc[i].mSize, nsbp, sizeof(sd.mCBDesc[i].mSize));
					nsbp += sizeof(sd.mCBDesc[i].mSize);
					sd.mCBDesc[i].mSize = LE2Native(sd.mCBDesc[i].mSize);

					uint16_t var_desc_size;
					std::memcpy(&var_desc_size, nsbp, sizeof(var_desc_size));
					nsbp += sizeof(var_desc_size);
					var_desc_size = LE2Native(var_desc_size);
					sd.mCBDesc[i].mVarDesc.resize(var_desc_size);
					for (size_t j = 0; j < sd.mCBDesc[i].mVarDesc.size(); ++j)
					{
						len = *nsbp;
						++nsbp;
						sd.mCBDesc[i].mVarDesc[j].name.resize(len);
						std::memcpy(&sd.mCBDesc[i].mVarDesc[j].name[0], nsbp, len);
						nsbp += len;

						std::memcpy(&sd.mCBDesc[i].mVarDesc[j].start_offset, nsbp, sizeof(sd.mCBDesc[i].mVarDesc[j].start_offset));
						nsbp += sizeof(sd.mCBDesc[i].mVarDesc[j].start_offset);

						sd.mCBDesc[i].mVarDesc[j].start_offset = LE2Native(sd.mCBDesc[i].mVarDesc[j].start_offset);
						std::memcpy(&sd.mCBDesc[i].mVarDesc[j].type, nsbp, sizeof(sd.mCBDesc[i].mVarDesc[j].type));
						nsbp += sizeof(sd.mCBDesc[i].mVarDesc[j].type);
						std::memcpy(&sd.mCBDesc[i].mVarDesc[j].rows, nsbp, sizeof(sd.mCBDesc[i].mVarDesc[j].rows));
						nsbp += sizeof(sd.mCBDesc[i].mVarDesc[j].rows);
						std::memcpy(&sd.mCBDesc[i].mVarDesc[j].columns, nsbp, sizeof(sd.mCBDesc[i].mVarDesc[j].columns));
						nsbp += sizeof(sd.mCBDesc[i].mVarDesc[j].columns);
						std::memcpy(&sd.mCBDesc[i].mVarDesc[j].elements, nsbp, sizeof(sd.mCBDesc[i].mVarDesc[j].elements));
						nsbp += sizeof(sd.mCBDesc[i].mVarDesc[j].elements);
						sd.mCBDesc[i].mVarDesc[j].elements = LE2Native(sd.mCBDesc[i].mVarDesc[j].elements);
					}
				}

				std::memcpy(&sd.mNumSamplers, nsbp, sizeof(sd.mNumSamplers));
				nsbp += sizeof(sd.mNumSamplers);
				sd.mNumSamplers = LE2Native(sd.mNumSamplers);
				std::memcpy(&sd.mNumSrvs, nsbp, sizeof(sd.mNumSrvs));
				nsbp += sizeof(sd.mNumSrvs);
				sd.mNumSrvs = LE2Native(sd.mNumSrvs);
				std::memcpy(&sd.mNumUAVS, nsbp, sizeof(sd.mNumUAVS));
				nsbp += sizeof(sd.mNumUAVS);
				sd.mNumUAVS = LE2Native(sd.mNumUAVS);

				uint16_t res_desc_size;
				std::memcpy(&res_desc_size, nsbp, sizeof(res_desc_size));
				nsbp += sizeof(res_desc_size);
				res_desc_size = LE2Native(res_desc_size);
				sd.mResDesc.resize(res_desc_size);
				for (size_t i = 0; i < sd.mResDesc.size(); ++i)
				{
					len = *nsbp;
					++nsbp;
					sd.mResDesc[i].name.resize(len);
					std::memcpy(&sd.mResDesc[i].name[0], nsbp, len);
					nsbp += len;

					std::memcpy(&sd.mResDesc[i].type, nsbp, sizeof(sd.mResDesc[i].type));
					nsbp += sizeof(sd.mResDesc[i].type);

					std::memcpy(&sd.mResDesc[i].dimension, nsbp, sizeof(sd.mResDesc[i].dimension));
					nsbp += sizeof(sd.mResDesc[i].dimension);

					std::memcpy(&sd.mResDesc[i].bindPoint, nsbp, sizeof(sd.mResDesc[i].bindPoint));
					nsbp += sizeof(sd.mResDesc[i].bindPoint);
					sd.mResDesc[i].bindPoint = LE2Native(sd.mResDesc[i].bindPoint);
				}

				if (ST_VertexShader == type)
				{
					std::memcpy(&mSoTemplate->mVsSignatrue, nsbp, sizeof(mSoTemplate->mVsSignatrue));
					nsbp += sizeof(mSoTemplate->mVsSignatrue);
					mSoTemplate->mVsSignatrue = LE2Native(mSoTemplate->mVsSignatrue);
				}
				else if (ST_ComputeShader == type)
				{
					std::memcpy(&mCSBlockSizeX, nsbp, sizeof(mCSBlockSizeX));
					nsbp += sizeof(mCSBlockSizeX);
					mCSBlockSizeX = LE2Native(mCSBlockSizeX);

					std::memcpy(&mCSBlockSizeY, nsbp, sizeof(mCSBlockSizeY));
					nsbp += sizeof(mCSBlockSizeY);
					mCSBlockSizeY = LE2Native(mCSBlockSizeY);

					std::memcpy(&mCSBlockSizeZ, nsbp, sizeof(mCSBlockSizeZ));
					nsbp += sizeof(mCSBlockSizeZ);
					mCSBlockSizeZ = LE2Native(mCSBlockSizeZ);
				}

				this->attachShaderBytecode(type, effect, shader_desc_ids, code_blob);

				ret = mIsShaderValidate[type];
			}
		}
		return ret;
	}

	bool D3D11ShaderObject::streamIn(ResIdentifierPtr const & res, ShaderType type, RenderEffect const & effect, std::array<uint32_t, ST_NumShaderTypes> const & shader_desc_ids)
	{
		uint32_t len;
		res->read(&len, sizeof(len));
		len = LE2Native(len);
		std::vector<uint8_t> native_shader_block(len);
		if (len > 0)
		{
			res->read(&native_shader_block[0], len * sizeof(native_shader_block[0]));
		}
		return this->attachNativeShader(type, effect, shader_desc_ids, native_shader_block);
	}

	void D3D11ShaderObject::streamOut(std::ostream & os, ShaderType type)
	{
		std::ostringstream oss(std::ios_base::binary | std::ios_base::out);

		{
			uint8_t len = static_cast<uint8_t>(mSoTemplate->mShaderCode[type].second.size());
			oss.write(reinterpret_cast<char const *>(&len), sizeof(len));
			oss.write(reinterpret_cast<char const *>(&mSoTemplate->mShaderCode[type].second[0]), len);
		}

		std::shared_ptr<std::vector<uint8_t>> code_blob = mSoTemplate->mShaderCode[type].first;
		if (code_blob)
		{
			uint8_t len;

			uint32_t blob_size = Native2LE(static_cast<uint32_t>(code_blob->size()));
			oss.write(reinterpret_cast<char const *>(&blob_size), sizeof(blob_size));
			oss.write(reinterpret_cast<char const *>(&((*code_blob)[0])), code_blob->size());

			auto const & sd = *mSoTemplate->mShaderDesc[type];

			uint16_t cb_desc_size = Native2LE(static_cast<uint16_t>(sd.mCBDesc.size()));
			oss.write(reinterpret_cast<char const *>(&cb_desc_size), sizeof(cb_desc_size));
			for (size_t i = 0; i < sd.mCBDesc.size(); ++i)
			{
				len = static_cast<uint8_t>(sd.mCBDesc[i].mName.size());
				oss.write(reinterpret_cast<char const *>(&len), sizeof(len));
				oss.write(reinterpret_cast<char const *>(&sd.mCBDesc[i].mName[0]), len);

				uint32_t size = Native2LE(sd.mCBDesc[i].mSize);
				oss.write(reinterpret_cast<char const *>(&size), sizeof(size));

				uint16_t var_desc_size = Native2LE(static_cast<uint16_t>(sd.mCBDesc[i].mVarDesc.size()));
				oss.write(reinterpret_cast<char const *>(&var_desc_size), sizeof(var_desc_size));
				for (size_t j = 0; j < sd.mCBDesc[i].mVarDesc.size(); ++j)
				{
					len = static_cast<uint8_t>(sd.mCBDesc[i].mVarDesc[j].name.size());
					oss.write(reinterpret_cast<char const *>(&len), sizeof(len));
					oss.write(reinterpret_cast<char const *>(&sd.mCBDesc[i].mVarDesc[j].name[0]), len);

					uint32_t start_offset = Native2LE(sd.mCBDesc[i].mVarDesc[j].start_offset);
					oss.write(reinterpret_cast<char const *>(&start_offset), sizeof(start_offset));
					oss.write(reinterpret_cast<char const *>(&sd.mCBDesc[i].mVarDesc[j].type), sizeof(sd.mCBDesc[i].mVarDesc[j].type));
					oss.write(reinterpret_cast<char const *>(&sd.mCBDesc[i].mVarDesc[j].rows), sizeof(sd.mCBDesc[i].mVarDesc[j].rows));
					oss.write(reinterpret_cast<char const *>(&sd.mCBDesc[i].mVarDesc[j].columns),
						sizeof(sd.mCBDesc[i].mVarDesc[j].columns));
					uint16_t elements = Native2LE(sd.mCBDesc[i].mVarDesc[j].elements);
					oss.write(reinterpret_cast<char const *>(&elements), sizeof(elements));
				}
			}

			uint16_t num_samplers = Native2LE(sd.mNumSamplers);
			oss.write(reinterpret_cast<char const *>(&num_samplers), sizeof(num_samplers));
			uint16_t num_srvs = Native2LE(sd.mNumSrvs);
			oss.write(reinterpret_cast<char const *>(&num_srvs), sizeof(num_srvs));
			uint16_t num_uavs = Native2LE(sd.mNumUAVS);
			oss.write(reinterpret_cast<char const *>(&num_uavs), sizeof(num_uavs));

			uint16_t res_desc_size = Native2LE(static_cast<uint16_t>(sd.mResDesc.size()));
			oss.write(reinterpret_cast<char const *>(&res_desc_size), sizeof(res_desc_size));
			for (size_t i = 0; i < sd.mResDesc.size(); ++i)
			{
				len = static_cast<uint8_t>(sd.mResDesc[i].name.size());
				oss.write(reinterpret_cast<char const *>(&len), sizeof(len));
				oss.write(reinterpret_cast<char const *>(&sd.mResDesc[i].name[0]), len);

				oss.write(reinterpret_cast<char const *>(&sd.mResDesc[i].type), sizeof(sd.mResDesc[i].type));
				oss.write(reinterpret_cast<char const *>(&sd.mResDesc[i].dimension), sizeof(sd.mResDesc[i].dimension));
				uint16_t bind_point = Native2LE(sd.mResDesc[i].bindPoint);
				oss.write(reinterpret_cast<char const *>(&bind_point), sizeof(bind_point));
			}

			if (ST_VertexShader == type)
			{
				uint32_t vs_signature = Native2LE(mSoTemplate->mVsSignatrue);
				oss.write(reinterpret_cast<char const *>(&vs_signature), sizeof(vs_signature));
			}
			else if (ST_ComputeShader == type)
			{
				uint32_t cs_block_size_x = Native2LE(mCSBlockSizeX);
				oss.write(reinterpret_cast<char const *>(&cs_block_size_x), sizeof(cs_block_size_x));

				uint32_t cs_block_size_y = Native2LE(mCSBlockSizeY);
				oss.write(reinterpret_cast<char const *>(&cs_block_size_y), sizeof(cs_block_size_y));

				uint32_t cs_block_size_z = Native2LE(mCSBlockSizeZ);
				oss.write(reinterpret_cast<char const *>(&cs_block_size_z), sizeof(cs_block_size_z));
			}
		}

		std::string native_shader_block = oss.str();
		uint32_t len = static_cast<uint32_t>(native_shader_block.size());
		{
			uint32_t tmp = Native2LE(len);
			os.write(reinterpret_cast<char const *>(&tmp), sizeof(tmp));
		}
		if (len > 0)
		{
			os.write(reinterpret_cast<char const *>(&native_shader_block[0]), len * sizeof(native_shader_block[0]));
		}
	}

	void D3D11ShaderObject::attachShader(ShaderType type, RenderEffect const & effect, RenderTechnique const & tech, RenderPass const & pass, std::array<uint32_t, ST_NumShaderTypes> const & shader_desc_ids)
	{
		std::shared_ptr < std::vector<uint8_t>> code_blob = this->compiteToBytecode(type, effect, tech, pass, shader_desc_ids);
		this->attachShaderBytecode(type, effect, shader_desc_ids, code_blob);
	}

	void D3D11ShaderObject::attachShader(ShaderType type, RenderEffect const & effect, RenderTechnique const & tech, RenderPass const & pass, ShaderObjectPtr const & shared_so)
	{
		if (shared_so)
		{
			D3D11ShaderObject const & so = *checked_cast<D3D11ShaderObject*>(shared_so.get());
			mIsShaderValidate[type] = so.mIsShaderValidate[type];
			mSoTemplate->mShaderCode[type] = so.mSoTemplate->mShaderCode[type];

			mSoTemplate->mShaderDesc[type] = so.mSoTemplate->mShaderDesc[type];
			switch (type)
			{
			case Air::ShaderObject::ST_VertexShader:
				mSoTemplate->mVertexShader = so.mSoTemplate->mVertexShader;
				mSoTemplate->mVsSignatrue = so.mSoTemplate->mVsSignatrue;
				mSoTemplate->mGeometryShader = so.mSoTemplate->mGeometryShader;
				break;
			case Air::ShaderObject::ST_PixelShader:
				mSoTemplate->mPixelShader = so.mSoTemplate->mPixelShader;
				break;
			case Air::ShaderObject::ST_GeometryShader:
				mSoTemplate->mGeometryShader = so.mSoTemplate->mGeometryShader;
				break;
			case Air::ShaderObject::ST_ComputeShader:
				mSoTemplate->mComputeShader = so.mSoTemplate->mComputeShader;
				mCSBlockSizeX = so.mCSBlockSizeX;
				mCSBlockSizeY = so.mCSBlockSizeY;
				mCSBlockSizeZ = so.mCSBlockSizeZ;
				break;
			case Air::ShaderObject::ST_HullShader:
				mSoTemplate->mHullShader = so.mSoTemplate->mHullShader;
				if (mSoTemplate->mHullShader)
				{
					mHasTessellation = true;
				}
				break;
			case Air::ShaderObject::ST_DomainShader:
				mSoTemplate->mDomainShader = so.mSoTemplate->mDomainShader;
				mSoTemplate->mGeometryShader = so.mSoTemplate->mGeometryShader;
				if (mSoTemplate->mDomainShader)
				{
					mHasTessellation = true;
				}
				break;
			default:
				mIsShaderValidate[type] = false;
				break;
			}
			mSamplers[type] = so.mSamplers[type];
			mSRVSrcs[type].resize(so.mSRVS[type].size(), std::make_tuple(static_cast<void*>(nullptr), 0, 0));
			mSRVS[type].resize(so.mSRVS[type].size());
			if (so.mSoTemplate->mShaderDesc[type]->mNumUAVS > 0)
			{
				mUAVSRCS.resize(so.mUAVS.size(), nullptr);
				mUAVS.resize(so.mUAVS.size());
			}
			mSoTemplate->mCBufferIndices[type] = so.mSoTemplate->mCBufferIndices[type];

			mD3DCBuffers[type].resize(so.mD3DCBuffers[type].size());
			mParamBinds[type].reserve(so.mParamBinds[type].size());
			for (auto const & pb : so.mParamBinds[type])
			{
				mParamBinds[type].push_back(this->getBindFunc(type, pb.offset, pb.param));
			}
		}

	}

	void D3D11ShaderObject::linkShaders(RenderEffect const & effect)
	{
		std::vector<uint32_t> all_cbuff_indices;
		RenderEnvironment &env = SingletonManager::getRenderFactoryInstance().getRenderEngineInstance().getRenderEnvironment();
		mIsvalidate = true;
		for (size_t type = 0; type < ShaderObject::ST_NumShaderTypes; ++type)
		{
			mIsvalidate &= mIsShaderValidate[type];
			if (mSoTemplate->mCBufferIndices[type] && !mSoTemplate->mCBufferIndices[type]->empty())
			{
				all_cbuff_indices.insert(all_cbuff_indices.end(), mSoTemplate->mCBufferIndices[type]->begin(), mSoTemplate->mCBufferIndices[type]->end());
				for (size_t i = 0; i < mSoTemplate->mCBufferIndices[type]->size(); ++i)
				{
					auto cbuff = effect.getCBufferByIndex((*mSoTemplate->mCBufferIndices[type])[i]);
					if (cbuff->getType() == CBT_Object)
					{
						cbuff->resize(mSoTemplate->mShaderDesc[type]->mCBDesc[i].mSize);
						BOOST_ASSERT(cbuff->getNumParameters() == mSoTemplate->mShaderDesc[type]->mCBDesc[i].mVarDesc.size());
						for (uint32_t j = 0; j < cbuff->getNumParameters(); ++j)
						{
							RenderEffectParameter* param = effect.getParameterByIndex(cbuff->getParametersIndex(j));
							uint32_t stride;
							if (mSoTemplate->mShaderDesc[type]->mCBDesc[i].mVarDesc[j].elements > 0)
							{
								if (param->getType() != REDT_float4x4)
								{
									stride = 16;
								}
								else
								{
									stride = 64;
								}
							}
							else
							{
								if (param->getType() != REDT_float4x4)
								{
									stride = 4;
								}
								else
								{
									stride = 16;
								}
							}
							param->bindToCBuffer(*cbuff, mSoTemplate->mShaderDesc[type]->mCBDesc[i].mVarDesc[j].start_offset, stride);

						}
					}
					else
					{
						SharedConstantBuffer* scb = env.getConstantBuffer(cbuff->getName());
						if (!scb->isInit())
						{
							cbuff->resize(mSoTemplate->mShaderDesc[type]->mCBDesc[i].mSize);
							BOOST_ASSERT(scb->getNumParams() == mSoTemplate->mShaderDesc[type]->mCBDesc[i].mVarDesc.size());
							for (uint32_t j = 0; j < scb->getNumParams(); ++j)
							{
								RenderEffectParameter* param = scb->getParameterByIndex(j);
								uint32_t stride;
								if (mSoTemplate->mShaderDesc[type]->mCBDesc[i].mVarDesc[j].elements > 0)
								{
									if (param->getType() != REDT_float4x4)
									{
										stride = 16;
									}
									else
									{
										stride = 64;
									}
								}
								else
								{
									if (param->getType() != REDT_float4x4)
									{
										stride = 4;
									}
									else
									{
										stride = 16;
									}
								}
								param->bindToCBuffer(*cbuff, mSoTemplate->mShaderDesc[type]->mCBDesc[i].mVarDesc[j].start_offset, stride);
							}
							scb->setInit();
						}
					}
					
					mD3DCBuffers[type][i] = checked_cast<D3D11GraphicsBuffer*>(cbuff->getHWbuffer().get())->getD3DBuffer();
				}
			}
		}
		std::sort(all_cbuff_indices.begin(), all_cbuff_indices.end());
		all_cbuff_indices.erase(std::unique(all_cbuff_indices.begin(), all_cbuff_indices.end()), all_cbuff_indices.end());
		
		mAllCBuffers.resize(all_cbuff_indices.size());
		for (size_t i = 0; i < all_cbuff_indices.size(); ++i)
		{
			mAllCBuffers[i] = effect.getCBufferByIndex(all_cbuff_indices[i]);
		}
	}


	ShaderObjectPtr D3D11ShaderObject::clone(RenderEffect const & effect)
	{
		D3D11ShaderObjectPtr ret = MakeSharedPtr<D3D11ShaderObject>(mSoTemplate);

		ret->mHasDiscard = mHasDiscard;
		ret->mHasTessellation = mHasTessellation;
		ret->mIsvalidate = mIsvalidate;
		ret->mIsShaderValidate = mIsShaderValidate;
		ret->mCSBlockSizeX = mCSBlockSizeX;
		ret->mCSBlockSizeY = mCSBlockSizeY;
		ret->mCSBlockSizeZ = mCSBlockSizeZ;
		ret->mUAVSRCS.resize(mUAVSRCS.size(), nullptr);
		ret->mUAVS.resize(mUAVS.size());
		std::vector<uint32_t> all_cbuff_indices;
		for (size_t i = 0; i < ST_NumShaderTypes; ++i)
		{
			ret->mSamplers[i] = mSamplers[i];
			ret->mSRVSrcs[i].resize(mSRVSrcs[i].size());
			ret->mSRVS[i].resize(mSRVS[i].size());
			if (mSoTemplate->mCBufferIndices[i] && !mSoTemplate->mCBufferIndices[i]->empty())
			{
				ret->mD3DCBuffers[i].resize(mD3DCBuffers[i].size());
				all_cbuff_indices.insert(all_cbuff_indices.end(), mSoTemplate->mCBufferIndices[i]->begin(), mSoTemplate->mCBufferIndices[i]->end());
				for (size_t j = 0; j < mSoTemplate->mCBufferIndices[i]->size(); ++j)
				{
					auto cbuff = effect.getCBufferByIndex((*mSoTemplate->mCBufferIndices[i])[j]);
					ret->mD3DCBuffers[i][j] = checked_cast<D3D11GraphicsBuffer*>(cbuff->getHWbuffer().get())->getD3DBuffer();
				}
			}
			ret->mParamBinds[i].reserve(mParamBinds[i].size());
			for (auto const & pb : mParamBinds[i])
			{
				ret->mParamBinds[i].push_back(ret->getBindFunc(static_cast<ShaderType>(i), pb.offset, effect.getParameterByName(pb.param->getName())));
			}
		}
		std::sort(all_cbuff_indices.begin(), all_cbuff_indices.end());
		all_cbuff_indices.erase(std::unique(all_cbuff_indices.begin(), all_cbuff_indices.end()), all_cbuff_indices.end());
		ret->mAllCBuffers.resize(all_cbuff_indices.size());
		for (size_t i = 0; i < all_cbuff_indices.size(); ++i)
		{
			ret->mAllCBuffers[i] = effect.getCBufferByIndex(all_cbuff_indices[i]);
		}
		return ret;
	}

	D3D11ShaderObject::ParameterBind D3D11ShaderObject::getBindFunc(ShaderType type, uint32_t offset, RenderEffectParameter* param)
	{
		ParameterBind ret;
		ret.param = param;
		ret.offset = offset;

		switch (param->getType())
		{
		case REDT_bool:
		case REDT_uint:
		case REDT_int:
		case REDT_float:
		case REDT_uint2:
		case REDT_uint3:
		case REDT_uint4:
		case REDT_int2:
		case REDT_int3:
		case REDT_int4:
		case REDT_float2:
		case REDT_float3:
		case REDT_float4:
		case REDT_float4x4:
		case REDT_sampler:
			break;

		case REDT_texture1D:
		case REDT_texture2D:
		case REDT_texture3D:
		case REDT_textureCUBE:
		case REDT_texture1DArray:
		case REDT_texture2DArray:
		case REDT_texture3DArray:
		case REDT_textureCUBEArray:
			ret.func = SetD3D11ShaderParameterTextureSRV(mSRVSrcs[type][offset], mSRVS[type][offset], param);
			break;

		case REDT_buffer:
		case REDT_structured_buffer:
		case REDT_consume_structured_buffer:
		case REDT_append_structured_buffer:
		case REDT_byte_address_buffer:
			ret.func = SetD3D11ShaderParameterGraphicsBufferSRV(mSRVSrcs[type][offset], mSRVS[type][offset], param);
			break;

		case REDT_rw_texture1D:
		case REDT_rw_texture2D:
		case REDT_rw_texture3D:
		case REDT_rw_texture1DArray:
		case REDT_rw_texture2DArray:
			ret.func = SetD3D11ShaderParameterTextureUAV(mUAVSRCS[offset], mUAVS[offset], param);
			break;

		case REDT_rw_buffer:
		case REDT_rw_structured_buffer:
		case REDT_rw_byte_address_buffer:
			ret.func = SetD3D11ShaderParameterGraphicsBufferUAV(mUAVSRCS[offset], mUAVS[offset], param);
			break;

		default:
			AIR_UNREACHABLE("Invalid type");
		}

		return ret;
	}

	void D3D11ShaderObject::bind()
	{
		D3D11RenderEngine& re = *checked_cast<D3D11RenderEngine*>(&SingletonManager::getRenderFactoryInstance().getRenderEngineInstance());
		re.setVertexShader(mSoTemplate->mVertexShader.get());
		re.setGeometryShader(mSoTemplate->mGeometryShader.get());
		re.setPixelShader(mSoTemplate->mPixelShader.get());
		re.setComputeShader(mSoTemplate->mComputeShader.get());
		re.setHullShader(mSoTemplate->mHullShader.get());
		re.setDomainShader(mSoTemplate->mDomainShader.get());
		for (auto const & pbs : mParamBinds)
		{
			for (auto const & pb : pbs)
			{
				pb.func();
			 }
		}


		for (auto cb : mAllCBuffers)
		{
			cb->update();
		}

		for (size_t st = 0; st < ST_NumShaderTypes; ++st)
		{
			if (!mSRVS[st].empty())
			{
				re.setShaderResources(static_cast<ShaderObject::ShaderType>(st), mSRVSrcs[st], mSRVS[st]);
			}
			if (!mSamplers[st].empty())
			{
				re.setSamplers(static_cast<ShaderObject::ShaderType>(st), mSamplers[st]);
			}
			if (!mD3DCBuffers[st].empty())
			{
				re.setConstantBuffers(static_cast<ShaderObject::ShaderType>(st), mD3DCBuffers[st]);
			}
		}
		if (mSoTemplate->mComputeShader && !mUAVS.empty())
		{
			for (uint32_t i = 0; i < mUAVS.size(); ++i)
			{
				if (mUAVSRCS[i] != nullptr)
				{
					re.detachSRV(mUAVSRCS[i], 0, 1);
				}
			}
			std::vector<UINT> uav_init_counts(mUAVS.size(), 0);
			re.csSetUnorderedAccessViews(0, static_cast<UINT>(mUAVS.size()), &mUAVS[0], &uav_init_counts[0]);
		}

	}
	void D3D11ShaderObject::unbind()
	{
		D3D11RenderEngine& re = *checked_cast<D3D11RenderEngine*>(&SingletonManager::getRenderFactoryInstance().getRenderEngineInstance());
		if (mSoTemplate->mComputeShader && !mUAVS.empty())
		{
			std::vector<ID3D11UnorderedAccessView*> uavs(mUAVS.size(), nullptr);
			std::vector<UINT> uav_init_counts(uavs.size(), 0);
			re.csSetUnorderedAccessViews(0, static_cast<UINT>(uavs.size()), &uavs[0], &uav_init_counts[0]);
		}
	}
	std::shared_ptr<std::vector<uint8_t>> const & D3D11ShaderObject::getVSCode() const
	{
		return mSoTemplate->mShaderCode[ST_VertexShader].first;
	}

	uint32_t D3D11ShaderObject::getVSSignature() const
	{
		return mSoTemplate->mVsSignatrue;
	}

	std::string_view D3D11ShaderObject::getShaderProfile(ShaderType type, RenderEffect const & effect, uint32_t shader_desc_id)
	{
		auto const & sd = effect.getShaderDesc(shader_desc_id);
		auto const & re = *checked_cast<D3D11RenderEngine const *>(&SingletonManager::getRenderFactoryInstance().getRenderEngineInstance());
		auto const & caps = re.getDeviceCaps();
		std::string_view shader_prefile = sd.mProfile;
		size_t const shader_profile_hash = boost::hash_range(shader_prefile.begin(), shader_prefile.end());
		switch (type)
		{
		case Air::ShaderObject::ST_VertexShader:
			if (CT_HASH("auto") == shader_profile_hash)
			{
				shader_prefile = re.getVertexShaderProfile();
			}
			break;
		case Air::ShaderObject::ST_PixelShader:
			if (CT_HASH("auto") == shader_profile_hash)
			{
				shader_prefile = re.getPixelShaderProfile();
			}
			break;
		case Air::ShaderObject::ST_GeometryShader:
			if (caps.mGSSupport)
			{
				if (CT_HASH("auto") == shader_profile_hash)
				{
					shader_prefile = re.getGeometryShaderProfile();
				}
			}
			break;
		case Air::ShaderObject::ST_ComputeShader:
			if (caps.mCSSupport)
			{
				if (CT_HASH("auto") == shader_profile_hash)
				{
					shader_prefile = re.getComputeShaderProfile();
				}
			}
			break;
		case Air::ShaderObject::ST_HullShader:
			if (caps.mHSSupport)
			{
				if (CT_HASH("auto") == shader_profile_hash)
				{
					shader_prefile = re.getHullShaderProfile();
				}
			}
			break;
		case Air::ShaderObject::ST_DomainShader:
		{
			if (caps.mDSSupport)
			{
				if (CT_HASH("auto") == shader_profile_hash)
				{
					return re.getDomainShaderProfile();
				}
			}
		}
			break;
		default:
			break;
		}
		return shader_prefile;
	}

	std::shared_ptr<std::vector<uint8_t>> D3D11ShaderObject::compiteToBytecode(ShaderType type, RenderEffect const & effect, RenderTechnique const & tech, RenderPass const & pass, std::array<uint32_t, ST_NumShaderTypes> const & ShaderDescIds)
	{
#ifdef AIR_PLATFORM_WINDOWS_DESKTOP
		auto const & re = *checked_cast<D3D11RenderEngine const*>(&SingletonManager::getRenderFactoryInstance().getRenderEngineInstance());
		auto const & caps = re.getDeviceCaps();

		auto const & sd = effect.getShaderDesc(ShaderDescIds[type]);
		mIsShaderValidate[type] = true;
		char const* shader_profile = sd.mProfile.c_str();
		size_t const shader_profile_hash = RT_HASH(shader_profile);
		switch (type)
		{
		case Air::ShaderObject::ST_VertexShader:
			if (CT_HASH("auto") == shader_profile_hash)
			{
				shader_profile = re.getVertexShaderProfile();
			}
			break;
		case Air::ShaderObject::ST_PixelShader:
			if (CT_HASH("auto") == shader_profile_hash)
			{
				shader_profile = re.getPixelShaderProfile();
			}
			break;
		case Air::ShaderObject::ST_GeometryShader:
			if (caps.mGSSupport)
			{
				if (CT_HASH("auto") == shader_profile_hash)
				{
					shader_profile = re.getGeometryShaderProfile();
				}
			}
			else
			{
				mIsShaderValidate[type] = false;
			}
			break;
		case Air::ShaderObject::ST_ComputeShader:
			if (caps.mCSSupport)
			{
				if (CT_HASH("auto") == shader_profile_hash)
				{
					shader_profile = re.getComputeShaderProfile();
				}
				if ((CT_HASH("cs_5_0") == shader_profile_hash) && (caps.mMaxShaderModel < ShaderModel(5, 0)))
				{
					mIsShaderValidate[type] = false;
				}
			}
			else
			{
				mIsShaderValidate[type] = false;
			}
			break;
		case Air::ShaderObject::ST_HullShader:
			if (caps.mHSSupport)
			{
				if (CT_HASH("auto") == shader_profile_hash)
				{
					shader_profile = re.getHullShaderProfile();
				}
			}
			else
			{
				mIsShaderValidate[type] = false;
			}
			break;
		case Air::ShaderObject::ST_DomainShader:
			if (caps.mDSSupport)
			{
				if (CT_HASH("auto") == shader_profile_hash)
				{
					shader_profile = re.getDomainShaderProfile();
				}
			}
			else
			{
				mIsShaderValidate[type] = false;
			}
			break;
		default:
			mIsShaderValidate[type] = false;
			break;
		}
		mSoTemplate->mShaderCode[type].second = shader_profile;
		std::shared_ptr<std::vector<uint8_t>> code = MakeSharedPtr<std::vector<uint8_t>>();
		if (mIsShaderValidate[type])
		{
			std::vector<std::pair<char const *, char const *>> macros;
			macros.emplace_back("AIR_D3D11", "1");
			macros.emplace_back("AIR_FRAG_DEPTH", "1");
			uint32_t flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if  !defined(AIR_DEBUG)
			flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif
			*code = this->compileToDXBC(type, effect, tech, pass, macros, sd.mFunctionName.c_str(), shader_profile, flags);
			if (!code->empty())
			{
				ID3D11ShaderReflection* reflection;
				this->reflectionDXBC(*code, reinterpret_cast<void**>(&reflection));
				if (reflection != nullptr)
				{
					if (!mSoTemplate->mShaderDesc[type])
					{
						mSoTemplate->mShaderDesc[type] = MakeSharedPtr<D3D11ShaderObjectTemplate::D3D11ShaderDesc>();
					}
					D3D11_SHADER_DESC desc;
					reflection->GetDesc(&desc);
					for (UINT o = 0; o < desc.OutputParameters; ++o)
					{
						D3D11_SIGNATURE_PARAMETER_DESC odc;
						reflection->GetOutputParameterDesc(0, &odc);
						std::string name = odc.SemanticName;


					}
					for (UINT c = 0; c < desc.ConstantBuffers; ++c)
					{
						ID3D11ShaderReflectionConstantBuffer* reflection_cb = reflection->GetConstantBufferByIndex(c);
						D3D11_SHADER_BUFFER_DESC d3d_cb_desc;
						reflection_cb->GetDesc(&d3d_cb_desc);
						if ((D3D_CT_CBUFFER == d3d_cb_desc.Type) || (D3D_CT_TBUFFER == d3d_cb_desc.Type))
						{
							D3D11ShaderObjectTemplate::D3D11ShaderDesc::ConstantBufferDesc cb_desc;
							cb_desc.mName = d3d_cb_desc.Name;
							cb_desc.mNameHash = RT_HASH(d3d_cb_desc.Name);
							cb_desc.mSize = d3d_cb_desc.Size;
							for (UINT v = 0; v < d3d_cb_desc.Variables; ++v)
							{
								ID3D11ShaderReflectionVariable* reflection_var = reflection_cb->GetVariableByIndex(v);
								D3D11_SHADER_VARIABLE_DESC var_desc;
								reflection_var->GetDesc(&var_desc);

								D3D11_SHADER_TYPE_DESC type_desc;
								reflection_var->GetType()->GetDesc(&type_desc);

								D3D11ShaderObjectTemplate::D3D11ShaderDesc::ConstantBufferDesc::VariableDesc vd;
								vd.name = var_desc.Name;
								vd.start_offset = var_desc.StartOffset;
								vd.type = static_cast<uint8_t>(type_desc.Type);
								vd.rows = static_cast<uint8_t>(type_desc.Rows);
								vd.columns = static_cast<uint8_t>(type_desc.Columns);
								vd.elements = static_cast<uint16_t>(type_desc.Elements);
								cb_desc.mVarDesc.push_back(vd);
							}
							mSoTemplate->mShaderDesc[type]->mCBDesc.push_back(cb_desc);
						}
					}
					int max_sampler_bind_pt = -1;
					int max_srv_bind_pt = -1;
					int max_uav_bind_pt = -1;
					for (uint32_t i = 0; i < desc.BoundResources; ++i)
					{
						D3D11_SHADER_INPUT_BIND_DESC si_desc;
						reflection->GetResourceBindingDesc(i, &si_desc);
						switch (si_desc.Type)
						{
						case D3D_SIT_SAMPLER:
							max_sampler_bind_pt = std::max(max_sampler_bind_pt, static_cast<int>(si_desc.BindPoint));
							break;
						case D3D_SIT_TEXTURE:
						case D3D_SIT_STRUCTURED:
						case D3D_SIT_BYTEADDRESS:
							max_srv_bind_pt = std::max(max_srv_bind_pt, static_cast<int>(si_desc.BindPoint));
							break;

						case D3D_SIT_UAV_RWTYPED:
						case D3D_SIT_UAV_RWSTRUCTURED:
						case D3D_SIT_UAV_RWBYTEADDRESS:
						case D3D_SIT_UAV_APPEND_STRUCTURED:
						case D3D_SIT_UAV_CONSUME_STRUCTURED:
						case D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER:
							max_uav_bind_pt = std::max(max_uav_bind_pt, static_cast<int>(si_desc.BindPoint));
							break;
						default:
							break;
						}
					}
					mSoTemplate->mShaderDesc[type]->mNumSamplers = static_cast<uint16_t>(max_sampler_bind_pt + 1);
					mSoTemplate->mShaderDesc[type]->mNumSrvs = static_cast<uint16_t>(max_srv_bind_pt + 1);
					mSoTemplate->mShaderDesc[type]->mNumUAVS = static_cast<uint16_t>(max_uav_bind_pt + 1);

					for (uint32_t i = 0; i < desc.BoundResources; ++i)
					{
						D3D11_SHADER_INPUT_BIND_DESC si_desc;
						reflection->GetResourceBindingDesc(i, &si_desc);
						switch (si_desc.Type)
						{
						case D3D_SIT_TEXTURE:
						case D3D_SIT_SAMPLER:
						case D3D_SIT_STRUCTURED:
						case D3D_SIT_BYTEADDRESS:
						case D3D_SIT_UAV_RWTYPED:
						case D3D_SIT_UAV_RWSTRUCTURED:
						case D3D_SIT_UAV_RWBYTEADDRESS:
						case D3D_SIT_UAV_APPEND_STRUCTURED:
						case D3D_SIT_UAV_CONSUME_STRUCTURED:
						case D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER:
							if (effect.getParameterByName(si_desc.Name))
							{
								D3D11ShaderObjectTemplate::D3D11ShaderDesc::BoundResourceDesc brd;
								brd.name = si_desc.Name;
								brd.type = static_cast<uint8_t>(si_desc.Type);
								brd.bindPoint = static_cast<uint16_t>(si_desc.BindPoint);
								mSoTemplate->mShaderDesc[type]->mResDesc.push_back(brd);
							}
							break;
						default:
							break;
						}
					}
					if (ST_VertexShader == type)
					{
						mSoTemplate->mVsSignatrue = 0;
						D3D11_SIGNATURE_PARAMETER_DESC signature;
						for (uint32_t i = 0; i < desc.InputParameters; ++i)
						{
							reflection->GetInputParameterDesc(i, &signature);
							size_t seed = RT_HASH(signature.SemanticName);
							boost::hash_combine(seed, signature.SemanticIndex);
							boost::hash_combine(seed, signature.Register);
							boost::hash_combine(seed, static_cast<uint32_t>(signature.SystemValueType));
							boost::hash_combine(seed, static_cast<uint32_t>(signature.ComponentType));
							boost::hash_combine(seed, signature.Mask);
							boost::hash_combine(seed, signature.ReadWriteMask);
							boost::hash_combine(seed, signature.Stream);
							boost::hash_combine(seed, signature.MinPrecision);
							size_t sig = mSoTemplate->mVsSignatrue;
							boost::hash_combine(sig, seed);
							mSoTemplate->mVsSignatrue = static_cast<uint32_t>(sig);
						}
					}
					else if (ST_ComputeShader == type)
					{
						reflection->GetThreadGroupSize(&mCSBlockSizeX, &mCSBlockSizeY, &mCSBlockSizeZ);
					}
					reflection->Release();
				}
				*code = this->stripDXBC(*code, D3DCOMPILER_STRIP_REFLECTION_DATA | D3DCOMPILER_STRIP_DEBUG_INFO | D3DCOMPILER_STRIP_TEST_BLOBS | D3DCOMPILER_STRIP_PRIVATE_DATA);
			}
		}
		if (code->empty())
		{
			code.reset();
		}
		return code;
#else
AIR_UNUSED(type);
AIR_UNUSED(effect);
AIR_UNUSED(tech);
AIR_UNUSED(pass);
AIR_UNUSED(shader_desc_ids);

return std::shared_ptr<std::vector<uint8_t>>();
#endif
	}

	void D3D11ShaderObject::attachShaderBytecode(ShaderType type, RenderEffect const & effect, std::array<uint32_t, ST_NumShaderTypes> const & shader_desc_ids, std::shared_ptr<std::vector<uint8_t>> const & code_blob)
	{
		if (code_blob)
		{
			RenderFactory& rf = SingletonManager::getRenderFactoryInstance();
			D3D11RenderEngine const & re = *checked_cast<D3D11RenderEngine const*>(&rf.getRenderEngineInstance());
			ID3D11Device* d3d_device = re.getD3DDevice();
			RenderDeviceCaps const & caps = re.getDeviceCaps();
			ShaderDesc const & sd = effect.getShaderDesc(shader_desc_ids[type]);
			uint8_t shader_major_ver = ("auto" == sd.mProfile) ? 0 : static_cast<uint8_t>(sd.mProfile[3] - '0');
			uint8_t shader_minor_ver = ("auto" == sd.mProfile) ? 0 : static_cast<uint8_t>(sd.mProfile[5] - '0');
			if (ShaderModel(shader_major_ver, shader_minor_ver) > caps.mMaxShaderModel)
			{
				mIsShaderValidate[type] = false;
			}
			else
			{
				switch (type)
				{
				case Air::ShaderObject::ST_VertexShader:
				{

					ID3D11VertexShader* vs;
					if (FAILED(d3d_device->CreateVertexShader(&((*code_blob)[0]), code_blob->size(), nullptr, &vs)))
					{
						mIsShaderValidate[type] = false;
					}
					else
					{
						mSoTemplate->mVertexShader = MakeComPtr(vs);
						if (!sd.mSODecl.empty())
						{
							if (caps.mGSSupport)
							{
								std::vector<D3D11_SO_DECLARATION_ENTRY> d3d11_decl(sd.mSODecl.size());
								for (size_t i = 0; i < sd.mSODecl.size(); ++i)
								{
									d3d11_decl[i] = D3D11Mapping::mapping(sd.mSODecl[i]);
								}
								UINT rasterized_stream = 0;
								if ((caps.mMaxShaderModel >= ShaderModel(5, 0)) && (effect.getShaderDesc(shader_desc_ids[ShaderObject::ST_PixelShader]).mFunctionName.empty()))
								{
									rasterized_stream = D3D11_SO_NO_RASTERIZED_STREAM;
								}
								ID3D11GeometryShader* gs;
								if (FAILED(d3d_device->CreateGeometryShaderWithStreamOutput(&((*code_blob)[0]), code_blob->size(), &d3d11_decl[0], static_cast<UINT>(d3d11_decl.size()), nullptr, 0, rasterized_stream, nullptr, &gs)))
								{
									mIsShaderValidate[type] = false;
								}
								else
								{
									mSoTemplate->mGeometryShader = MakeComPtr(gs);
								}
							}
							else
							{
								mIsShaderValidate[type] = false;
							}
						}
						mSoTemplate->mShaderCode[type].first = code_blob;
					}
				}
					break;
				case Air::ShaderObject::ST_PixelShader:
				{
					ID3D11PixelShader* ps;
					if (FAILED(d3d_device->CreatePixelShader(&((*code_blob)[0]), code_blob->size(), nullptr, &ps)))
					{
						mIsShaderValidate[type] = false;
					}
					else
					{
						mSoTemplate->mPixelShader = MakeComPtr(ps);
						mSoTemplate->mShaderCode[type].first = code_blob;
					}
				}
					break;
				case Air::ShaderObject::ST_GeometryShader:
					if (caps.mGSSupport)
					{
						if (sd.mSODecl.empty())
						{
							ID3D11GeometryShader* gs;
							if (FAILED(d3d_device->CreateGeometryShader(&((*code_blob)[0]), code_blob->size(), nullptr, &gs)))
							{
								mIsShaderValidate[type] = false;
							}
							else
							{
								mSoTemplate->mGeometryShader = MakeComPtr(gs);
								mSoTemplate->mShaderCode[type].first = code_blob;
							}
						}
						else
						{
							std::vector<D3D11_SO_DECLARATION_ENTRY> d3d11_decl(sd.mSODecl.size());
							for (size_t i = 0; i < sd.mSODecl.size(); ++i)
							{
								d3d11_decl[i] = D3D11Mapping::mapping(sd.mSODecl[i]);
							}
							UINT rasterized_stream = 0;
							if ((caps.mMaxShaderModel >= ShaderModel(5, 0)) && (effect.getShaderDesc(shader_desc_ids[ShaderObject::ST_PixelShader]).mFunctionName.empty()))
							{
								rasterized_stream = D3D11_SO_NO_RASTERIZED_STREAM;
							}
							ID3D11GeometryShader* gs;
							if (FAILED(d3d_device->CreateGeometryShaderWithStreamOutput(&((*code_blob)[0]), code_blob->size(), &d3d11_decl[0], static_cast<UINT>(d3d11_decl.size()), nullptr, 0, rasterized_stream, nullptr, &gs)))
							{
								mIsShaderValidate[type] = false;
							}
							else
							{
								mSoTemplate->mGeometryShader = MakeComPtr(gs);
								mSoTemplate->mShaderCode[type].first = code_blob;
							}
						}
					}
					else
					{
						mIsShaderValidate[type] = false;
					}
					break;
				case Air::ShaderObject::ST_ComputeShader:
					if (caps.mCSSupport)
					{
						ID3D11ComputeShader* cs;
						if (FAILED(d3d_device->CreateComputeShader(&((*code_blob)[0]), code_blob->size(), nullptr, &cs)))
						{
							mIsShaderValidate[type] = false;
						}
						else
						{
							mSoTemplate->mComputeShader = MakeComPtr(cs);
							mSoTemplate->mShaderCode[type].first = code_blob;
						}
					}
					else
					{
						mIsShaderValidate[type] = false;
					}
					break;
				case Air::ShaderObject::ST_HullShader:
					if (caps.mHSSupport)
					{
						ID3D11HullShader* hs;
						if (FAILED(d3d_device->CreateHullShader(&((*code_blob)[0]), code_blob->size(), nullptr, &hs)))
						{
							mIsShaderValidate[type] = false;
						}
						else
						{
							mSoTemplate->mHullShader = MakeComPtr(hs);
							mSoTemplate->mShaderCode[type].first = code_blob;
							mHasTessellation = true;
						}
					}
					else
					{
						mIsShaderValidate[type] = false;
					}
					break;
				case Air::ShaderObject::ST_DomainShader:
					if (caps.mDSSupport)
					{
						ID3D11DomainShader* ds;
						if (FAILED(d3d_device->CreateDomainShader(&((*code_blob)[0]), code_blob->size(), nullptr, &ds)))
						{
							mIsShaderValidate[type] = false;
						}
						else
						{
							mSoTemplate->mDomainShader = MakeComPtr(ds);
							if (!sd.mSODecl.empty())
							{
								if (caps.mGSSupport)
								{
									std::vector<D3D11_SO_DECLARATION_ENTRY> d3d11_decl(sd.mSODecl.size());
									for (size_t i = 0; i < sd.mSODecl.size(); ++i)
									{
										d3d11_decl[i] = D3D11Mapping::mapping(sd.mSODecl[i]);
									}
									UINT rasterized_stream = 0;
									if ((caps.mMaxShaderModel >= ShaderModel(5, 0)) && (effect.getShaderDesc(shader_desc_ids[ShaderObject::ST_PixelShader]).mFunctionName.empty()))
									{
										rasterized_stream = D3D11_SO_NO_RASTERIZED_STREAM;
									}
									ID3D11GeometryShader* gs;
									if (FAILED(d3d_device->CreateGeometryShaderWithStreamOutput(&((*code_blob)[0]), code_blob->size(), &d3d11_decl[0], static_cast<UINT>(d3d11_decl.size()), nullptr, 0, rasterized_stream, nullptr, &gs)))
									{
										mIsShaderValidate[type] = false;
									}
									else
									{
										mSoTemplate->mGeometryShader = MakeComPtr(gs);
									}
								}
								else
								{
									mIsShaderValidate[type] = false;
								}
							}
							mSoTemplate->mShaderCode[type].first = code_blob;
							mHasTessellation = true;
						}
					}
					else
					{
						mIsShaderValidate[type] = false;
					}
					break;
				default:
					mIsShaderValidate[type] = false;
					break;
				}
			}
			if (!mSoTemplate->mShaderDesc[type]->mCBDesc.empty())
			{
				mSoTemplate->mCBufferIndices[type] = MakeSharedPtr<std::vector<uint8_t>>(mSoTemplate->mShaderDesc[type]->mCBDesc.size());
			}
			mD3DCBuffers[type].resize(mSoTemplate->mShaderDesc[type]->mCBDesc.size());
			for (size_t c = 0; c < mSoTemplate->mShaderDesc[type]->mCBDesc.size(); ++c)
			{
				uint32_t i = 0;
				for (; i < effect.getNumCBuffers(); ++i)
				{
					if (effect.getCBufferByIndex(i)->getNameHash() == mSoTemplate->mShaderDesc[type]->mCBDesc[c].mNameHash)
					{
						(*mSoTemplate->mCBufferIndices[type])[c] = static_cast<uint8_t>(i);
						break;
					}
				}
				BOOST_ASSERT(i < effect.getNumCBuffers());
			}
			mSamplers[type].resize(mSoTemplate->mShaderDesc[type]->mNumSamplers);
			mSRVSrcs[type].resize(mSoTemplate->mShaderDesc[type]->mNumSrvs, std::make_tuple(static_cast<void*>(nullptr), 0, 0));
			mSRVS[type].resize(mSoTemplate->mShaderDesc[type]->mNumSrvs);
			mUAVSRCS.resize(mSoTemplate->mShaderDesc[type]->mNumUAVS, nullptr);
			mUAVS.resize(mSoTemplate->mShaderDesc[type]->mNumUAVS);
			for (size_t i = 0; i < mSoTemplate->mShaderDesc[type]->mResDesc.size(); ++i)
			{
				RenderEffectParameter* p = effect.getParameterByName(mSoTemplate->mShaderDesc[type]->mResDesc[i].name);
				BOOST_ASSERT(p);
				uint32_t offset = mSoTemplate->mShaderDesc[type]->mResDesc[i].bindPoint;
				if (D3D_SIT_SAMPLER == mSoTemplate->mShaderDesc[type]->mResDesc[i].type)
				{
					SamplerStateObjectPtr sampler;
					p->getValue(sampler);
					if (sampler)
					{
						mSamplers[type][offset] = checked_cast<D3D11SamplerStateObject*>(sampler.get())->getD3DSamplerState();
					}
				}
				else
				{
					mParamBinds[type].push_back(this->getBindFunc(type, offset, p));
				}
			}
		}
		else
		{
			mIsShaderValidate[type] = false;
		}
	}
}
