#ifndef _UTIL_H_
#define _UTIL_H_
namespace Air
{
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