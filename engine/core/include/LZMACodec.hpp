#ifndef _LZMACodec_H_
#define _LZMACodec_H_
#pragma once

#include "PreDeclare.h"
namespace Air
{
	class AIR_CORE_API LZMACodec
	{
	public:
		LZMACodec();
		~LZMACodec();

		uint64_t encode();

	};
}




#endif