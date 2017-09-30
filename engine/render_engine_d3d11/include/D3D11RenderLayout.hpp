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


		void active() const;

		std::vector<ID3D11Buffer*> const & getVBs() const
		{
			return mVBs;
		}

		std::vector<UINT> const & getStrides() const
		{
			return mStrides;
		}

		std::vector<UINT> const & getOffsets() const
		{
			return mOffsets;
		}
		ID3D11InputLayout* getInputLayout(ShaderObject const * so) const;

	private:
		mutable std::vector<D3D11_INPUT_ELEMENT_DESC> mVertexElement;
		mutable std::vector<std::pair<size_t, ID3D11InputLayoutPtr>> mInputLayput;
		mutable std::vector<ID3D11Buffer*> mVBs;
		mutable std::vector<UINT> mStrides;
		mutable std::vector<UINT> mOffsets;
	};
}
#endif