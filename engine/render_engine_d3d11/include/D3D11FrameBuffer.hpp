#ifndef _D3D11_Frame_Buffer_H_
#define _D3D11_Frame_Buffer_H_
#pragma once
#include "rendersystem/include/FrameBuffer.hpp"
#include "render_engine_d3d11/include/D3D11Typedefs.hpp"

namespace Air
{
	class D3D11FrameBuffer : public FrameBuffer
	{
	public:
		D3D11FrameBuffer();
		virtual ~D3D11FrameBuffer();

		ID3D11RenderTargetView* getD3DRTView(uint32_t n) const;
		ID3D11DepthStencilView* getD3DDSView() const;
		ID3D11UnorderedAccessView* getD3DUAView(uint32_t n) const;

		virtual std::wstring const & getDescription() const;

		virtual void onBind();
		virtual void onUnbind();


		void clear(uint32_t flags, Color const &clr, float depth, int32_t stencil);
		virtual void discard(uint32_t flags) override;
	private:
		D3D11_VIEWPORT mD3DViewport;
	};
}


#endif