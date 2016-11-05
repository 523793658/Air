#ifndef _UTIL_H_
#define _UTIL_H_
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

	std::string& convert(std::string& strDest, std::string const & strSrc);
	std::string& convert(std::string& strDest, std::wstring const & wstrSrc);
	std::wstring& convert(std::wstring & wstrDest, std::string const & strSrc);
	std::wstring& convert(std::wstring& wstrDst, std::wstring const & wstrSrc);
}
#endif