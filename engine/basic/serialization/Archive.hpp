#ifndef _Archive_H_
#define _Archive_H_
#pragma once
#include "basic/include/Basic.h"
#include <unordered_map>
namespace Air
{

	class Archive
	{
	public:
		Archive();
		Archive(const Archive &);

		Archive& operator = (const Archive & rhs);

		virtual bool close();

		virtual ~Archive();
	public:
		virtual Archive& operator << (std::string_view);

		virtual Archive& operator << (std::wstring_view);

		virtual Archive& operator << (int32_t value);
	};

}
#endif
