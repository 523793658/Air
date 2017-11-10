#include "Context.h"
#include <dxgiformat.h>

#include "render_engine_d3d11/include/D3D11VideoMode.hpp"


namespace Air
{
	D3D11VideoMode::D3D11VideoMode()
		:mWidth(0), mHeight(0), mFormat(DXGI_FORMAT_UNKNOWN)
	{

	}

	D3D11VideoMode::D3D11VideoMode(uint32_t width, uint32_t height, DXGI_FORMAT format)
		:mWidth(width), mHeight(height), mFormat(format)
	{

	}

	uint32_t D3D11VideoMode::getWidth() const
	{
		return mWidth;
	}

	uint32_t D3D11VideoMode::getHeight() const
	{
		return mHeight;
	}
	DXGI_FORMAT D3D11VideoMode::getFormat() const
	{
		return mFormat;
	}

	uint32_t D3D11VideoMode::getColorDepth() const
	{
		uint32_t colorDepth;
		if (mFormat == DXGI_FORMAT_B8G8R8A8_UNORM ||
			mFormat == DXGI_FORMAT_B8G8R8A8_UNORM_SRGB ||
			mFormat == DXGI_FORMAT_R8G8B8A8_UNORM ||
			mFormat == DXGI_FORMAT_R8G8B8A8_UNORM_SRGB ||
			mFormat == DXGI_FORMAT_R10G10B10A2_UNORM)
		{
			colorDepth = 32;
		}
		else
		{
			colorDepth = 16;
		}
		return colorDepth;
	}

	bool operator < (D3D11VideoMode const & lhs, D3D11VideoMode const & rhs)
	{
		if (lhs.getWidth() < rhs.getWidth())
		{
			return true;
		}
		else
		{
			if (lhs.getWidth() == rhs.getWidth())
			{
				if (lhs.getHeight() < rhs.getHeight())
				{
					return true;
				}
				else
				{
					if (lhs.getHeight() == rhs.getHeight())
					{
						if (lhs.getFormat() < rhs.getFormat())
						{
							return true;
						}
					}
				}
			}
		}
		return false;
	}

	bool operator == (D3D11VideoMode const &lhs, D3D11VideoMode const &rhs)
	{
		return (lhs.getWidth() == rhs.getWidth() && lhs.getHeight() == rhs.getHeight() && lhs.getFormat() == rhs.getFormat());
	}

}