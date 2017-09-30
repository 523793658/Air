#ifndef _TexCompression_BC_H_
#define _TexCompression_BC_H_
#pragma once

#include "core/include/PreDeclare.h"
#include <array>
namespace Air
{
#ifdef AIR_HAS_STRUCT_PACK
#pragma pack(push, 1)
#endif
	struct BC1Block
	{
		uint16_t color0, color1;
		uint16_t bitmap[2];
	};

	struct BC2Block
	{
		uint16_t alpha[4];
		BC1Block bc1;
	};

	struct BC4Block
	{
		uint8_t alpha_0, alpha_1;
		uint8_t bitmap[6];
	};

	struct BC3Block
	{
		BC4Block alpha;
		BC1Block bc1;
	};

	struct BC5Block
	{
		BC4Block red;
		BC4Block green;
	};
#ifdef AIR_HAS_STRUCT_PACK
#pragma pack(pop)
#endif


	void BC4ToBC1G(BC1Block& bc1, BC4Block const & bc4)
	{
		bc1.color0 = (bc4.alpha_0 >> 2) << 5;
		bc1.color1 = (bc4.alpha_1 >> 2) << 5;
		bool swap_clr = false;
		if (bc4.alpha_0 < bc4.alpha_1)
		{
			swap_clr = true;
		}
		for (int i = 0; i < 2; ++i)
		{
			uint32_t alpha32 = (bc4.bitmap[i * 3 + 2] << 16) | (bc4.bitmap[i * 3 + 1] << 8) | (bc4.bitmap[i * 3 + 0] << 0);
			uint16_t mask = 0;
			for (int j = 0; j < 8; ++j)
			{
				uint16_t bit = (alpha32 >> (j * 3)) & 0x7;
				if (swap_clr)
				{
					switch (bit)
					{
					case 0:
					case 6:
						bit = 0;
						break;
					case 1: 
					case 7:
						bit = 1;
						break;
					case 2:
					case 3:
						bit = 2;
						break;
					case 4:
					case 5:
						bit = 3;
						break;
					}
				}
				else
				{
					switch (bit)
					{
					case 0:
					case 2:
						bit = 0;
						break;
					case 1:
					case 5:
					case 7:
						bit = 1;
						break;
					case 3:
					case 4:
					case 6: 
						bit = 2;
						break;
					}
				}
				mask |= bit << (j * 2);
			}
			bc1.bitmap[i] = mask;
		}
	}
}




#endif
