#include "basic/include/STLVector.hpp"

namespace Air
{
	template<typename T>
	void doSer(Archive& ar, std::vector<T> v)
	{
		ar << v.size();
		for (auto it : v)
		{
			ar << it;
		}
	}

	Archive& operator << (Archive & ar, std::vector<uint32_t> const & a)
	{
		doSer(ar, a);
		return ar;
	}

	Archive& operator << (Archive & ar, std::vector<std::string> const & a)
	{
		doSer(ar, a);
		return ar;

	}
}