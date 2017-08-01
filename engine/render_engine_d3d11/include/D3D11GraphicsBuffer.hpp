#ifndef _D3D11GrappicsBuffer_H_
#define _D3D11GrappicsBuffer_H_
#pragma once

#include "ElementFormat.h"
#include "rendersystem/include/GraphicsBuffer.hpp"
#include "D3D11Typedefs.hpp"

namespace Air
{
	class D3D11GraphicsBuffer : public GraphicsBuffer
	{
	public:
		D3D11GraphicsBuffer(BufferUsage usage, uint32_t access_hint, uint32_t bind_flags, uint32_t size_in_bytes, ElementFormat fmt);

		ID3D11Buffer* getD3DBuffer() const
		{
			return mBuffer.get();
		}
		ID3D11ShaderResourceViewPtr const & getD3DShaderResourceView() const
		{
			mD3DShaderResourceView;
		}
		ID3D11RenderTargetViewPtr const & getD3DRenderTargetView() const;
		ID3D11UnorderedAccessViewPtr const & getD3DUnorderAccessView() const
		{
			return mD3DUAView;
		}
		void copyToBuffer(GraphicsBuffer& rhs);
		void createHWResouce(void const * init_data) override;
		void deleteHWResource() override;
		void updateSubResource(uint32_t offset, uint32_t size, void const * data);
	protected:
		void getD3DFlags(D3D11_USAGE& usage, UINT& cpu_access_flags, UINT& bind_flags, UINT& misc_flags);
	private:
		void * map(BufferAccess ba);
		void unmap();
	private:
		ID3D11Device* mD3DDevice;
		ID3D11DeviceContext* mD3DImmCtx;
		ID3D11BufferPtr mBuffer;
		ID3D11ShaderResourceViewPtr mD3DShaderResourceView;
		mutable ID3D11RenderTargetViewPtr mD3DRTView;
		ID3D11UnorderedAccessViewPtr mD3DUAView;
		uint32_t mBindFlags;
		ElementFormat mFmtAsShaderRes;
	};
}


#endif