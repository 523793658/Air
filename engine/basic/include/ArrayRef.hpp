#ifndef _ArrayRef_HPP_
#define _ArrayRef_HPP_
#include "basic/include/Util.h"
#include <vector>
#include <boost/assert.hpp>
namespace Air
{
	template <typename T>
	class ArrayRef
	{
	public:
		typedef T const * iterator;
		typedef T const * const_iterator;
		typedef size_t size_type;
		typedef std::reverse_iterator<iterator> reverse_iterator;

	public:
		ArrayRef()
			:mData(nullptr), mSize(0)
		{

		}
		ArrayRef(ArrayRef const & rhs)
			:mData(rhs.data()), mSize(rhs.size())
		{

		}
		AIR_IMPLICIT ArrayRef(T const & t)
			: mData(&t), mSize(1)
		{

		}

		ArrayRef(T const * data, size_t size)
			:mData(data), mSize(size)
		{

		}

		ArrayRef(T const * begin, T const * end)
			:mData(begin), mSize(end - begin)
		{

		}

		template<typename A>
		AIR_IMPLICIT ArrayRef(std::vector<T, A> const & v)
			:mData(v.data()), mSize(v.size())
		{

		}

		template<size_t N>
		AIR_IMPLICIT constexpr ArrayRef(T const (&arr)[N])
			:mData(arr), mSize(N)
		{

		}

		ArrayRef(std::initializer_list<T> const & v)
			:mData(v.begin() == v.end() ? nullptr : v.begin()), mSize(v.size())
		{

		}

		template<typename U>
		AIR_IMPLICIT ArrayRef(ArrayRef<U*> const & rhs, typename std::enable_if<std::is_convertible<U* const *, T const *>::value>::type* = 0)
		:mData(rhs.data()), mSize(rhs.size())
		{

		}


		template <typename U, typename A>
		AIR_IMPLICIT ArrayRef(std::vector<U*, A> const & v,
			typename std::enable_if<std::is_convertible<U* const *, T const *>::value>::type* = 0)
			: mData(v.data()), mSize(v.size())
		{
		}

		iterator begin() const
		{
			return mData;
		}
		iterator end() const
		{
			return mData + mSize;
		}

		reverse_iterator rbegin() const
		{
			return reverse_iterator(this->end());
		}
		reverse_iterator rend() const
		{
			return reverse_iterator(this->begin());
		}

		T const * data() const
		{
			return mData;
		}

		size_t size() const
		{
			return mSize;
		}

		bool empty() const
		{
			return mSize == 0;
		}

		T const & front() const
		{
			BOOST_ASSERT(!this->empty());
			return mData[0];
		}

		T const & back() const
		{
			BOOST_ASSERT(!this->empty());
			return mData[mSize - 1];
		}

		template <typename Alloc>
		ArrayRef<T> Copy(Alloc& alloc)
		{
			T* buff = alloc.template allocate<T>(mSize);
			std::uninitialized_copy(this->begin(), this->end(), buff);
			return ArrayRef<T>(buff, mSize);
		}

		ArrayRef<T> Slice(uint32_t n) const
		{
			BOOST_ASSERT_MSG(n <= this->size(), "Invalid specifier");
			return ArrayRef<T>(this->data() + n, this->size() - n);
		}

		ArrayRef<T> Slice(uint32_t n, uint32_t m) const
		{
			BOOST_ASSERT_MSG(n + m <= this->size(), "Invalid specifier");
			return ArrayRef<T>(this->data() + n, m);
		}

		ArrayRef<T> DropBack(uint32_t n = 1) const
		{
			BOOST_ASSERT_MSG(this->size() >= n, "Dropping more elements than exist");
			return this->Slice(0, this->Size() - n);
		}

		T const & operator[](size_t index) const
		{
			BOOST_ASSERT_MSG(index < mSize, "Invalid index!");
			return mData[index];
		}

		std::vector<T> ToVector() const
		{
			return std::vector<T>(mData, mData + mSize);
		}
	private:
		T const * mData;
		size_type mSize;

	};

	template <typename T>
	inline bool operator==(ArrayRef<T> lhs, ArrayRef<T> rhs)
	{
		if (lhs.size() != rhs.size())
		{
			return false;
		}
		return std::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template <typename T>
	inline bool operator!=(ArrayRef<T> lhs, ArrayRef<T> rhs)
	{
		return !(lhs == rhs);
	}
}




#endif // !_ArrayRef_HPP_
