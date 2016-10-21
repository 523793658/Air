#ifndef _Thread_H_
#define _Thread_H_
#pragma 
#include <thread>
#include <mutex>

#include <exception>
#include <vector>
#include <functional>

namespace Air
{
	struct void_t
	{
		typedef void_t type;
	};

	typedef std::thread::id thread_id;

	inline thread_id threadof(int)
	{
		return std::this_thread::get_id();
	}

	template<typename Joiner>
	inline thread_id threadof(Joiner const & j)
	{
		return j.get_thread_id();
	}

	class bad_join : public std::exception
	{
	public:
		const char* what() const AIR_NOEXCEPT
		{
			return "bad join";
		}
	};
}

#endif