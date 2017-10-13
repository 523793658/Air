#ifndef _D3D11ShaderObject_H_
#define _D3D11ShaderObject_H_
#pragma once
#include "PreDeclare.h"

#include "rendersystem/include/ShaderObject.hpp"

#include "render_engine_d3d11/include/D3D11Typedefs.hpp"


namespace Air
{
	class D3D11ShaderObject : public ShaderObject
	{
	public:
		D3D11ShaderObject();




		bool attachNativeShader(ShaderType type, RenderEffect const & effect, std::array<uint32_t, ST_NumShaderTypes> const & shader_desc_ids, std::vector<uint8_t> const & native_shader_block) override;

		bool streamIn(ResIdentifierPtr const & res, ShaderType type, RenderEffect const & effect, std::array<uint32_t, ST_NumShaderTypes> const & shader_desc_ids) override;

		void streamOut(std::ostream & os, ShaderType type)override;

		void attachShader(ShaderType type, RenderEffect const & effect, RenderTechnique const & tech, RenderPass const & pass, std::array<uint32_t, ST_NumShaderTypes> const & shader_desc_ids) override;

		void attachShader(ShaderType type, RenderEffect const & effect, RenderTechnique const & tech, RenderPass const & pass, ShaderObjectPtr const & shared_so) override;

		void linkShaders(RenderEffect const & effect) override;

		ShaderObjectPtr clone(RenderEffect const & effect) override;

		void bind() override;
		void unbind() override;

		std::shared_ptr<std::vector<uint8_t>> const & getVSCode() const;

		uint32_t getVSSignature() const;

	private:
		struct D3D11ShaderObjectTemplate
		{
#ifdef AIR_HAS_STRUCT_PACK
#pragma pack(push, 2)
#endif
			struct  D3D11ShaderDesc
			{
				D3D11ShaderDesc()
				{

				}

				struct ConstantBufferDesc
				{
					struct VariableDesc
					{
						std::string name;
						uint32_t start_offset;
						uint8_t type;
						uint8_t rows;
						uint8_t columns;
						uint16_t elements;
					};
					std::vector<VariableDesc> mVarDesc;
					std::string mName;
					size_t mNameHash;
					uint32_t mSize{ 0 };
				};
				std::vector<ConstantBufferDesc> mCBDesc;

				uint16_t mNumSamplers{ 0 };
				uint16_t mNumSrvs{ 0 };
				uint16_t mNumUAVS{ 0 };

				struct BoundResourceDesc
				{
					std::string name;
					uint8_t type;
					uint8_t dimension;
					uint16_t bindPoint;
				};
				std::vector<BoundResourceDesc> mResDesc;
			};
#ifdef AIR_HAS_STRUCT_PACK
#pragma pack(pop)
#endif
			ID3D11VertexShaderPtr mVertexShader;
			ID3D11PixelShaderPtr mPixelShader;
			ID3D11GeometryShaderPtr mGeometryShader;
			ID3D11ComputeShaderPtr mComputeShader;
			ID3D11HullShaderPtr mHullShader;
			ID3D11DomainShaderPtr mDomainShader;

			std::array<std::pair<std::shared_ptr<std::vector<uint8_t>>, std::string>, ST_NumShaderTypes> mShaderCode;

			std::array<std::shared_ptr<D3D11ShaderDesc>, ST_NumShaderTypes> mShaderDesc;

			std::array<std::shared_ptr<std::vector<uint8_t>>, ST_NumShaderTypes> mCBufferIndices;

			uint32_t mVsSignatrue;
		};

		struct ParameterBind
		{
			RenderEffectParameter* param;
			uint32_t offset;
			std::function<void()> func;
		};
	public:
		explicit D3D11ShaderObject(std::shared_ptr<D3D11ShaderObjectTemplate> const & so_template);

	private:
		ParameterBind getBindFunc(ShaderType type, uint32_t offset, RenderEffectParameter* param);

		std::string_view getShaderProfile(ShaderType type, RenderEffect const & effect, uint32_t shader_desc_id);

		std::shared_ptr<std::vector<uint8_t>> compiteToBytecode(ShaderType type, RenderEffect const & effect, RenderTechnique const & tech, RenderPass const & pass, std::array<uint32_t, ST_NumShaderTypes> const & ShaderDescIds);

		void attachShaderBytecode(ShaderType type, RenderEffect const & effect, std::array<uint32_t, ST_NumShaderTypes> const & shader_desc_ids, std::shared_ptr<std::vector<uint8_t>> const & code_blob);

	private:
		std::shared_ptr<D3D11ShaderObjectTemplate> mSoTemplate;
		std::array<std::vector<ParameterBind>, ST_NumShaderTypes> mParamBinds;

		std::array<std::vector<ID3D11SamplerState*>, ST_NumShaderTypes>
			mSamplers;
		std::array<std::vector<std::tuple<void*, uint32_t, uint32_t>>, ST_NumShaderTypes> mSRVSrcs;

		std::array<std::vector<ID3D11ShaderResourceView*>,
			ST_NumShaderTypes> mSRVS;

		std::array<std::vector<ID3D11Buffer*>, ST_NumShaderTypes> mD3DCBuffers;
		std::vector<void*> mUAVSRCS;
		std::vector<ID3D11UnorderedAccessView*> mUAVS;
		std::vector<RenderEffectConstantBuffer*> mAllCBuffers;

	};

	typedef std::shared_ptr<D3D11ShaderObject> D3D11ShaderObjectPtr;
}

#endif
