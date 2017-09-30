#include "Engine.h"
#include "rendersystem/include/RenderFactory.h"
#include "D3D11RenderEngine.hpp"
#include "D3D11ShaderObject.hpp"
#include "D3D11GraphicsBuffer.hpp"
#include "D3D11Mapping.hpp"
#include "D3D11RenderLayout.hpp"

namespace Air
{
	D3D11RenderLayout::D3D11RenderLayout()
	{

	}
	ID3D11InputLayout* D3D11RenderLayout::getInputLayout(ShaderObject const * so) const
	{
		if (!mVertexElement.empty())
		{
			D3D11ShaderObject const & shader = *checked_cast<D3D11ShaderObject const *>(so);
			auto const signature = shader.getVSSignature();
			for (auto const & il : mInputLayput)
			{
				if (il.first == signature)
				{
					return il.second.get();
				}
			}

			auto& vs_code = *shader.getVSCode();
			RenderFactory& rf = Engine::getInstance().getRenderFactoryInstance();
			D3D11RenderEngine& re = *checked_cast<D3D11RenderEngine*>(&rf.getRenderEngineInstance());
			ID3D11Device* d3d_device = re.getD3DDevice();
			ID3D11InputLayout* ia;
			TIFHR(d3d_device->CreateInputLayout(&mVertexElement[0], static_cast<UINT>(mVertexElement.size()), &vs_code[0], vs_code.size(), &ia));
			ID3D11InputLayoutPtr new_layout = MakeComPtr(ia);
			mInputLayput.emplace_back(signature, new_layout);
			return new_layout.get();
		}
		else
		{
			return nullptr;
		}
	}

	void D3D11RenderLayout::active() const
	{
		if (mStreamsDirty)
		{
			uint32_t const num_vertex_streams = this->getNumVertexStreams();
			uint32_t const all_num_vertex_stream = num_vertex_streams + (this->getInstanceStream() ? 1 : 0);
			mVertexElement.clear();
			mVertexElement.reserve(all_num_vertex_stream);
			mVBs.resize(all_num_vertex_stream);
			mStrides.resize(all_num_vertex_stream);
			mOffsets.resize(all_num_vertex_stream);
			for (uint32_t i = 0; i < num_vertex_streams; ++i)
			{
				std::vector<D3D11_INPUT_ELEMENT_DESC> stream_elems;
				D3D11Mapping::mapping(stream_elems, i, this->getVertexStreamFormat(i), mVertexStreams[i].mType, mVertexStreams[i].mFreq);
				mVertexElement.insert(mVertexElement.end(), stream_elems.begin(), stream_elems.end());
				GraphicsBuffer const * stream = this->getVertexStream(i).get();
				D3D11GraphicsBuffer const & d3dvb = *checked_cast<D3D11GraphicsBuffer const *>(stream);
				mVBs[i] = d3dvb.getD3DBuffer();
				mStrides[i] = this->getVertexSize(i);
				mOffsets[i] = 0;
			}
			if (this->getInstanceStream())
			{
				std::vector<D3D11_INPUT_ELEMENT_DESC> stream_elems;
				D3D11Mapping::mapping(stream_elems, this->getNumVertexStreams(), this->getInstanceStreamFormat(), mInstanceStream.mType, mInstanceStream.mFreq);
				mVertexElement.insert(mVertexElement.end(), stream_elems.begin(), stream_elems.end());
				uint32_t number = num_vertex_streams;
				GraphicsBuffer const * stream = this->getInstanceStream().get();
				D3D11GraphicsBuffer const & d3dvb = *checked_cast<D3D11GraphicsBuffer const *>(stream);
				mVBs[number] = d3dvb.getD3DBuffer();
				mStrides[number] = this->getInstanceSize();
				mOffsets[number] = 0;
			}
			mStreamsDirty = false;
		}
	}

}