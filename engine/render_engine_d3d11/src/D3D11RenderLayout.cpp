#include "Engine.h"

#include "D3D11RenderLayout.hpp"

namespace Air
{
	D3D11RenderLayout::D3D11RenderLayout()
	{

	}
	ID3D11InputLayout* D3D11RenderLayout::getInputLayout(size_t signature, std::vector<uint8_t> const & vs_code) const
	{
		//for (size_t i = 0; i < mInputLayput.size(); ++i)
		//{
		//	if (mInputLayput[i].first == signature)
		//	{
		//		return mInputLayput[i].second.get();
		//	}
		//}
		//std::vector<D3D11_INPUT_ELEMENT_DESC> elems;
		//elems.reserve(mVertexElement.size());
		//for (uint32_t i = 0; i < this->getNumVertexStreams(); ++i)
		//{
		//	std::vector<D3D11_INPUT_ELEMENT_DESC> stream_elems;
		//	//D3D11Mappnig
		//}
		return nullptr;
	}
}