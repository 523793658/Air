#ifndef _D3D11_Mapping_H_
#define _D3D11_Mapping_H_
#pragma once
#include "PreDeclare.h"
#include "ElementFormat.h"
#include "D3D11Typedefs.hpp"
namespace Air
{
	class D3D11Mapping
	{
	public:
		static uint32_t MappingColorMask(uint32_t mask);
		static DXGI_FORMAT MappingFormat(ElementFormat format);
		static ElementFormat MappingFormat(DXGI_FORMAT format);
	};
}




#endif