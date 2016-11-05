#ifndef _Basic_SIMDVector_H_
#define _Basic_SIMDVector_H_

#pragma once

#include <boost/operators.hpp>
#if !defined(AIR_COMPILER_MSVC) && !defined(AIR_CPU_ARM)
#include <x86intrin.h>
#endif

namespace KlayGE
{
#if defined(SIMD_MATH_SSE)
	typedef __m128 V4TYPE;
#else
	typedef std::array<float, 4> V4TYPE;
#endif

	class SIMDVectorF4 : boost::addable<SIMDVectorF4,
		boost::subtractable<SIMDVectorF4,
		boost::multipliable<SIMDVectorF4,
		boost::dividable<SIMDVectorF4,
		boost::dividable2<SIMDVectorF4, float,
		boost::multipliable2<SIMDVectorF4, float,
		boost::addable2<SIMDVectorF4, float,
		boost::subtractable2<SIMDVectorF4, float > > > > > > > >
	{
	public:
		SIMDVectorF4()
		{
		}
		SIMDVectorF4(SIMDVectorF4 const & rhs);

		static size_t size()
		{
			return 4;
		}

		static SIMDVectorF4 const & Zero();

		V4TYPE& Vec()
		{
			return vec_;
		}
		V4TYPE const & Vec() const
		{
			return vec_;
		}

		SIMDVectorF4 const & operator+=(SIMDVectorF4 const & rhs);
		SIMDVectorF4 const & operator+=(float rhs);
		SIMDVectorF4 const & operator-=(SIMDVectorF4 const & rhs);
		SIMDVectorF4 const & operator-=(float rhs);
		SIMDVectorF4 const & operator*=(SIMDVectorF4 const & rhs);
		SIMDVectorF4 const & operator*=(float rhs);
		SIMDVectorF4 const & operator/=(SIMDVectorF4 const & rhs);
		SIMDVectorF4 const & operator/=(float rhs);

		SIMDVectorF4& operator=(SIMDVectorF4 const & rhs);

		SIMDVectorF4 const operator+() const;
		SIMDVectorF4 const operator-() const;

		void swap(SIMDVectorF4& rhs);

	private:
		V4TYPE vec_;
	};

	inline void swap(SIMDVectorF4& lhs, SIMDVectorF4& rhs)
	{
		lhs.swap(rhs);
	}
}

#endif			// _KFL_SIMDVECTOR_HPP
