#include "basic/include/Basic.h"
#include "basic/include/Vector.hpp"
#include "basic/include/Color.hpp"
namespace Air
{
	template Color_T<float>::Color_T(float const * rhs) AIR_NOEXCEPT;
	template Color_T<float>::Color_T(Color const & rhs) AIR_NOEXCEPT;
	template Color_T<float>::Color_T(Color&& rhs) AIR_NOEXCEPT;
	template Color_T<float>::Color_T(float r, float g, float b, float a) AIR_NOEXCEPT;
	template Color_T<float>::Color_T(uint32_t dw) AIR_NOEXCEPT;
	template void Color_T<float>::getRGBA(uint8_t& R, uint8_t& g, uint8_t& b, uint8_t& a) const AIR_NOEXCEPT;

	template uint32_t Color_T<float>::getARGB() const AIR_NOEXCEPT;
	template uint32_t Color_T<float>::getABGR() const AIR_NOEXCEPT;

	template Color& Color_T<float>::operator +=(Color const & rhs) AIR_NOEXCEPT;
	template Color& Color_T<float>::operator -=(Color const & rhs) AIR_NOEXCEPT;
	template Color& Color_T<float>::operator *=(float rhs) AIR_NOEXCEPT;
	template Color& Color_T<float>::operator *=(Color const & rhs) AIR_NOEXCEPT;
	template Color& Color_T<float>::operator /=(float rhs) AIR_NOEXCEPT;

	template Color& Color_T<float>::operator = (Color const & rhs) AIR_NOEXCEPT;
	template Color& Color_T<float>::operator = (Color && rhs) AIR_NOEXCEPT;

	template Color const Color_T<float>::operator +() const AIR_NOEXCEPT;
	template Color const Color_T<float>::operator -() const AIR_NOEXCEPT;

	template bool Color_T<float>::operator==(Color const & rhs) const AIR_NOEXCEPT;


	template<typename T>
	Color_T<T>::Color_T(T const * rhs) AIR_NOEXCEPT
		:mCol(rhs)
	{

	}

	template<typename T>
	Color_T<T>::Color_T(Color_T const & rhs) AIR_NOEXCEPT
		:mCol(rhs.mCol)
	{

	}

	template<typename T>
	Color_T<T>::Color_T(Color_T && rhs) AIR_NOEXCEPT
		:mCol(std::move(rhs.mCol))
	{

	}

	template<typename T>
	Color_T<T>::Color_T(T r, T g, T b, T a) AIR_NOEXCEPT
	{
		mCol[0] = r;
		mCol[1] = g;
		mCol[2] = b;
		mCol[3] = a;
	}

	template<typename T>
	Color_T<T>::Color_T(uint32_t dw) AIR_NOEXCEPT
	{
		uint32_t x = dw;
		static T const f(1 / T(255));
		mCol[0] = f * (static_cast<T>(static_cast<uint8_t>(x >> 24)));
		mCol[0] = f * (static_cast<T>(static_cast<uint8_t>(x >> 16)));
		mCol[1] = f * (static_cast<T>(static_cast<uint8_t>(x >> 8)));
		mCol[2] = f * (static_cast<T>(static_cast<uint8_t>(x >> 0)));

	}

	template<typename T>
	void Color_T<T>::getRGBA(uint8_t& R, uint8_t& G, uint8_t& B, uint8_t& A) const AIR_NOEXCEPT
	{
		R = static_cast<uint8_t>(MathLib::clamp(this->r(), T(0), T(1)) * 255 + 0.5f);
		G = static_cast<uint8_t>(MathLib::clamp(this->g(), T(0), T(1)) * 255 + 0.5f);
		B = static_cast<uint8_t>(MathLib::clamp(this->b(), T(0), T(1)) * 255 + 0.5f);
		A = static_cast<uint8_t>(MathLib::clamp(this->a(), T(0), T(1)) * 255 + 0.5f);

	}

	template<typename T>
	uint32_t Color_T<T>::getARGB() const AIR_NOEXCEPT
	{
		uint8_t r, g, b, a;
		this->getRGBA(r, g, b, a);
		return (a << 24) | (r << 16) | (g << 8) | (b << 0);
	}

	template<typename T>
	uint32_t Color_T<T>::getABGR() const AIR_NOEXCEPT
	{
		uint8_t r, g, b, a;
		this->getRGBA(r, g, b, a);
		return (a << 24) | (b << 16) | (g << 8) | (r << 0);
	}

	template<typename T>
	Color_T<T>& Color_T<T>::operator +=(Color_T<T> const & rhs) AIR_NOEXCEPT
	{
		mCol + rhs.mCol;
		return *this;
	}

	template<typename T>
	Color_T<T>& Color_T<T>::operator -=(Color_T<T> const & rhs) AIR_NOEXCEPT
	{
		mCol -= rhs.mCol;
		return *this;

	}

	template<typename T>
	Color_T<T>& Color_T<T>::operator *=(T rhs) AIR_NOEXCEPT
	{
		mCol *= rhs;
		return *this;

	}

	template<typename T>
	Color_T<T>& Color_T<T>::operator *=(Color_T<T> const & rhs) AIR_NOEXCEPT
	{
		*this = MathLib::modulate(*this, rhs);
		return *this;
	}

	template<typename T>
	Color_T<T>& Color_T<T>::operator /=(T rhs) AIR_NOEXCEPT
	{
		mCol / rhs;
		return *this;
	}


	template<typename T>
	Color_T<T>& Color_T<T>::operator = (Color_T<T> const & rhs) AIR_NOEXCEPT
	{
		if (this != &rhs)
		{
			mCol = rhs.mCol;
		}
		return *this;
	}

	template<typename T>
	Color_T<T>& Color_T<T>::operator = (Color_T<T> && rhs) AIR_NOEXCEPT
	{
		mCol = std::move(rhs.mCol);
		return *this;
	}

	template<typename T>
	Color_T<T> const Color_T<T>::operator +() const AIR_NOEXCEPT
	{
		return *this;
	}

	template<typename T>
	Color_T<T> const Color_T<T>::operator -() const AIR_NOEXCEPT
	{
		return Color_T(-this->r(), -this->g(), -this->b(), -this->a());
	}

	template<typename T>
	bool Color_T<T>::operator==(Color_T<T> const & rhs) const AIR_NOEXCEPT
	{
		return mCol == rhs.mCol;
	}
}