#ifndef _Air_D3D11_Render_Layout_H_
#define _Air_D3D11_Render_Layout_H_
#pragma once

#include <vector>
#include "rendersystem/include/RenderLayout.hpp"
#include "D3D11Typedefs.hpp"

namespace Air
{
	class D3D11RenderLayout : public RenderLayout
	{
	public:
		D3D11RenderLayout();
		ID3D11InputLayout* getInputLayout(size_t signature, std::vector<uint8_t> const & vs_code) const;

	private:
		std::vector<D3D11_INPUT_ELEMENT_DESC> mVertexElement;
		mutable std::vector<std::pair<size_t, ID3D11InputLayoutPtr>> mInputLayput;
	};
}
#endif