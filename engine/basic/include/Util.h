#ifndef _UTIL_H_
#define _UTIL_H_
#include "basic/include/PreDeclare.h"
#include <string>
#include <basic/include/cpp17.h>
#include "boost/assert.hpp"




#define AIR_UNUSED(x) (void)(x)

#define AIR_IMPLICIT

#ifdef AIR_DEBUG
#define AIR_DBG_SUFFIX "_d"
#else
#define AIR_DBG_SUFFIX ""
#endif

#define AIR_OUTPUT_SUFFIX "_" KFL_STRINGIZE(AIR_COMPLIER_NAME) KFL_STRINGIZE(AIR_COMPILER_VERSION) AIR_DBG_SUFFIX

namespace Air
{

	template<typename T, typename... Args>
	inline std::unique_ptr<T> MakeUniquePtrHelper(std::false_type, Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	inline std::unique_ptr<T> MakeUniquePtrHelper(std::true_type, size_t size)
	{
		static_assert(0 == std::extent<T>::value, "make_unique<T[N]>() is forbidden, please use make_unique<T[]>().");
		return std::make_unique<T>(size)
	}

	template <typename T, typename... Args>
	inline std::shared_ptr<T> MakeSharedPtr(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	inline std::unique_ptr<T> MakeUniquePtr(Args&&... args)
	{
		return MakeUniquePtrHelper<T>(std::is_array<T>(), std::forward<Args>(args)...);
	}

	std::string readShortString(ResIdentifierPtr const & res);

	std::wstring readShortWString(ResIdentifierPtr const & res);

	void writeShortString(std::ostream& os, std::string_view str);


	// EndianµÄ×ª»»
	template <int size>
	void EndianSwitch(void* p);

	template<typename T>
	T Native2BE(T x)
	{
#ifdef AIR_LITTLE_ENDIAN
		EndianSwitch<sizeof(T)>(&x);
#else
		AIR_UNUSED(x);
#endif
		return x;
	}

	template<typename T>
	T Native2LE(T x)
	{
#ifdef AIR_LITTLE_ENDIAN
		AIR_UNUSED(x);
#
		EndianSwitch<sizeof(T)>(&x);
#endif
		return x;
	}

	template<typename T>
	T BE2Native(T x)
	{
		return Native2BE(x);
	}

	template <typename T>
	T LE2Native(T x)
	{
		return Native2LE(x);
	}

	template<uint8_t ch0, uint8_t ch1, uint8_t ch2, uint8_t ch3>
	struct MakeFourCC
	{
		enum { value = (ch0 << 0) + (ch1 << 8) + (ch2 << 16) + (ch3 << 24) };
	};

	std::string& convert(std::string& strDest, std::string const & strSrc);
	std::string& convert(std::string& strDest, std::wstring const & wstrSrc);
	std::wstring& convert(std::wstring & wstrDest, std::string const & strSrc);
	std::wstring& convert(std::wstring& wstrDst, std::wstring const & wstrSrc);

	template<typename To, typename From>
	inline To
		checked_cast(From p)
	{
		BOOST_ASSERT(dynamic_cast<To>(p) == static_cast<To>(p));
		return static_cast<To>(p);
	}

	template<typename To, typename From>
	inline std::shared_ptr<To>
		checked_pointer_cast(std::shared_ptr<From> const & p)
		{
			BOOST_ASSERT(std::dynamic_pointer_cast<To>(p) == std::static_pointer_cast<To>(p));
			return std::static_pointer_cast<To>(p);
		}

#define PRIME_NUM 0x9e3779b9

#ifdef AIR_CPP11_CORE_CONSTEXPR_SUPPORT
#ifdef AIR_COMPILER_MSVC
#pragma warning(disable: 4307)
#endif

	constexpr size_t _Hash(char const* str, size_t seed)
	{
		return 0 == *str ? seed : _Hash(str + 1, seed ^ (*str + PRIME_NUM + (seed << 6) + (seed >> 2)));
	}

#ifdef AIR_COMPILER_MSVC
	template<size_t N>
	struct EnsureConst
	{
		static const size_t value = N;
	};
#define CT_HASH(x) (EnsureConst<_Hash(x, 0)>::value)
#else
#define CT_HASH(x) (_Hash(x, 0))
#endif // AIR_COMPILER_MSVC
#else
#if defined(KLAYGE_COMPILER_MSVC)
#define FORCEINLINE __forceinline
#else
#define FORCEINLINE inline
#endif

	FORCEINLINE size_t _Hash(const char(&str)[1])
	{
		return *str + PRIME_NUM;
	}

	template <size_t N>
	FORCEINLINE size_t _Hash(const char(&str)[N])
	{
		typedef const char(&truncated_str)[N - 1];
		size_t seed = _Hash((truncated_str)str);
		return seed ^ (*(str + N - 1) + PRIME_NUM + (seed << 6) + (seed >> 2));
	}

	template <size_t N>
	FORCEINLINE size_t CT_HASH(const char(&str)[N])
	{
		typedef const char(&truncated_str)[N - 1];
		return _Hash((truncated_str)str);
	}

#undef FORCEINLINE
#endif
	inline size_t RT_HASH(char const * str)
	{
		size_t seed = 0;
		while (*str != 0)
		{
			seed ^= (*str + PRIME_NUM + (seed << 6) + (seed >> 2));
			++str;
		}
		return seed;
	}
#undef PRIME_NUM
}
#endif