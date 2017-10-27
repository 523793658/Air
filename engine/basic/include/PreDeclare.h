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

	class half;
	template <typename T, int N> 
	class Vector_T;
	typedef Vector_T<int, 1> int1;
	typedef Vector_T<int, 2> int2;
	typedef Vector_T<int, 3> int3;
	typedef Vector_T<int, 4> int4;

	typedef Vector_T<uint32_t, 1> uint1;
	typedef Vector_T<uint32_t, 2> uint2;
	typedef Vector_T<uint32_t, 3> uint3;
	typedef Vector_T<uint32_t, 4> uint4;

	typedef Vector_T<float, 1> float1;
	typedef Vector_T<float, 2> float2;
	typedef Vector_T<float, 3> float3;
	typedef Vector_T<float, 4> float4;

	template<typename T>
	class Color_T;
	typedef Color_T<float> Color;


	template <typename T>
	class Matrix4_T;
	typedef Matrix4_T<float> float4x4;

	template <typename T>
	class Plane_T;
	typedef Plane_T<float> Plane;

	template <typename T>
	class Frustum_T;
	typedef Frustum_T<float> Frustum;

	template<typename T>
	class Quaternion_T;
	typedef Quaternion_T<float> Quaternion;

	template<typename T>
	class AABBox_T;
	typedef AABBox_T<float> AABBox;
	typedef std::shared_ptr<AABBox> AABBoxPtr;



	class bad_join;
	class thread_pool;

	class Json;
	typedef std::shared_ptr<Json> JsonPtr;

	class JsonValue;
	typedef std::shared_ptr<JsonValue> JsonValuePtr;
}



#endif