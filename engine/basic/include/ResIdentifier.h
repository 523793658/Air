#ifndef _Basic_ResIdentifier_H_
#define _Basic_ResIdentifier_H_
#pragma once

#include <basic/include/PreDeclare.h>
#include <istream>
#include <vector>
#include <string>

namespace Air
{
	class ResIdentifier
	{
	public:
		ResIdentifier(std::string const &name, uint64_t timestamp,
			std::shared_ptr<std::istream> const & is);

		ResIdentifier(std::string const &name, uint64_t timestamp,
			std::shared_ptr<std::istream> & is, std::shared_ptr<std::streambuf> const & streambuf);

		void setResName(std::string const & name);

		std::string const & getResName() const;

		void setTimestamp(uint64_t ts);

		uint64_t getTimestamp() const;

		void read(void* p, size_t size);

		int64_t gcount() const;

		void seekg(int64_t offset, std::ios_base::seekdir way);

		int64_t tellg();

		void clear();

		uint32_t size()
		{
			int index = static_cast<int>(tellg());
			seekg(0, std::ios_base::end);
			uint32_t len = static_cast<uint32_t>(tellg());
			seekg(index, std::ios_base::beg);
			return len;
		}

		operator bool() const
		{
			return !mIstream->fail();
		}

		bool operator!() const
		{
			return mIstream->operator!();
		}


		std::istream& input_stream();

	private:
		std::string mName;
		uint64_t mTimestamp;
		std::shared_ptr<std::istream> mIstream;
		std::shared_ptr<std::streambuf> mIstreamBuf;
	};
}


#endif