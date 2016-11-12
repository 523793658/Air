#ifndef _D3D11VideoMode_H_
#define _D3D11VideoMode_H_
#pragma once
namespace Air
{
	class D3D11VideoMode
	{
	public:
		D3D11VideoMode();
		D3D11VideoMode(uint32_t width, uint32_t height, DXGI_FORMAT format);

		uint32_t getWidth() const;
		uint32_t getHeight() const;
		DXGI_FORMAT getFormat() const;
		uint32_t getColorDepth() const;

	private:
		uint32_t mWidth;
		uint32_t mHeight;
		DXGI_FORMAT mFormat;
	};

	bool operator <(D3D11VideoMode const & lhs, D3D11VideoMode const & rhs);
	bool operator == (D3D11VideoMode const &lhs, D3D11VideoMode const & rhs);
}



#endif