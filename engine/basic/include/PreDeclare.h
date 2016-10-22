#ifndef _BASIC_PREDECLARE_H_
#define _BASIC_PREDECLARE_H_
#pragma once

#ifdef AIR_COMPILER_GCC
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#include <memory>


#ifdef KLAYGE_COMPILER_GCC
#pragma GCC diagnostic pop
#endif

namespace Air
{
	class ResIdentifier;
	typedef std::shared_ptr<ResIdentifier> ResIdentifierPtr;
	class DllLoader;

	class XMLDocument;
	typedef std::shared_ptr<XMLDocument> XMLDocumentPtr;
	class XMLNode;
	typedef std::shared_ptr<XMLNode> XMLNodePtr;
	class XMLAttribute;
	typedef std::shared_ptr<XMLAttribute> XMLAttributePtr;

	class bad_join;


	class half;
	template<typename T, int N>
	class Vector_T;
	typedef Vector_T<int32_t, 1> int1;
	typedef Vector_T<int32_t, 2> int2;
	typedef Vector_T<int32_t, 3> int3;
	typedef Vector_T<int32_t, 4> int4;

	typedef Vector_T<uint32_t, 1> uint1;
	typedef Vector_T<uint32_t, 2> uint2;
	typedef Vector_T<uint32_t, 3> uint3;
	typedef Vector_T<uint32_t, 4> uint4;

	typedef Vector_T<float, 1> float1;
	typedef Vector_T<float, 2> float2;
	typedef Vector_T<float, 3> float3;
	typedef Vector_T<float, 4> float4;

}



#endif