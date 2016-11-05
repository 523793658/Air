#include "basic/include/Basic.h"

#include "basic/include/ResIdentifier.h"

namespace Air
{
	ResIdentifier::ResIdentifier(std::string const &name, uint64_t timestamp,
		std::shared_ptr<std::istream> const & is)
		:mName(name), mTimestamp(timestamp), mIstream(is)

	{

	}

	ResIdentifier::ResIdentifier(std::string const &name, uint64_t timestamp,
		std::shared_ptr<std::istream> & is, std::shared_ptr<std::streambuf> const & streambuf)
		:mName(name), mTimestamp(timestamp), mIstream(is), mIstreamBuf(streambuf)
	{

	}

	void ResIdentifier::setResName(std::string const & name)
	{
		mName = name;
	}

	std::string const & ResIdentifier::getResName() const
	{
		return mName;
	}

	void ResIdentifier::setTimestamp(uint64_t ts)
	{
		mTimestamp = ts;
	}

	uint64_t ResIdentifier::getTimestamp() const
	{
		return mTimestamp;
	}

	void ResIdentifier::read(void* p, size_t size)
	{
		mIstream->read(static_cast<char*>(p), static_cast<std::streamsize>(size));
	}

	int64_t ResIdentifier::gcount() const
	{
		return static_cast<int64_t>(mIstream->gcount());
	}

	void ResIdentifier::seekg(int64_t offset, std::ios_base::seekdir way)
	{
		mIstream->seekg(static_cast<std::istream::off_type>(offset), way);
	}

	int64_t ResIdentifier::tellg()
	{
		return static_cast<int64_t>(mIstream->tellg());
	}

	void ResIdentifier::clear()
	{
		mIstream->clear();
	}

	std::istream& ResIdentifier::input_stream()
	{
		return *mIstream;
	}


}