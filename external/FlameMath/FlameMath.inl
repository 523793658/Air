

#pragma once


const float MathHelper::Pi = 3.1415926f;

#pragma region Float2
/****************************************************************************
 *
 * Float2
 *
 ****************************************************************************/
inline bool Float2::operator == ( const Float2& V ) const
{
	return (x == V.x) && (y == V.y);
}

inline bool Float2::operator != ( const Float2& V ) const
{
	return (x!=V.x) || (y != V.y);
}

inline Float2& Float2::operator+= (const Float2& V)
{
	x += V.x;
	y += V.y;
    return *this;
}

inline Float2& Float2::operator-= (const Float2& V)
{
	x -= V.x;
	y -= V.y;
	return *this;
}

inline Float2& Float2::operator*= (const Float2& V)
{
	x *= V.x;
	y *= V.y;
	return *this;
}

inline Float2& Float2::operator*= (float S)
{
	x *= S;
	y *= S;
	return *this;
} 

inline Float2& Float2::operator/= (float S)
{
	float inv = 1.0f / S;
	x *= inv;
	y *= inv;
	return *this;
} 

inline Float2 operator+ (const Float2& V1, const Float2& V2)
{
    return Float2(V1.x + V2.x,V1.y + V2.y);
}

inline Float2 operator- (const Float2& V1, const Float2& V2)
{
    return Float2(V1.x - V2.x,V1.y - V2.y);
}

inline Float2 operator* (const Float2& V1, const Float2& V2)
{
	return Float2(V1.x * V2.x,V1.y * V2.y);
}

inline Float2 operator* (const Float2& V, float S)
{
    return Float2(V.x * S,V.y * S);
}

inline Float2 operator/ (const Float2& V1, const Float2& V2)
{
	return Float2(V1.x / V2.x,V1.y / V2.y);
}

inline Float2 operator* (float S, const Float2& V)
{
   return Float2(V.x * S,V.y * S);
}

inline bool Float2::InBounds( const Float2& Bounds ) const
{
	return ( (x <= Bounds.x && x >= -Bounds.x) 
		&& (y <= Bounds.y && y >= -Bounds.y));
}

inline float Float2::Length() const
{
   return sqrtf(x*x + y*y);
}

inline float Float2::LengthSquared() const
{
  return (x*x + y*y);
}

inline float Float2::Dot( const Float2& V ) const
{
   return (x * V.x + y * V.y);
}

inline void Float2::Cross( const Float2& V, Float2& result ) const
{
	float cross = x * V.y - y * V.x;
	result.x = cross;
	result.y = cross;
}

inline Float2 Float2::Cross( const Float2& V ) const
{
	float cross = x * V.y - y * V.x;
   return Float2(cross);
}

inline void Float2::Normalize()
{
	float len = Length();
	if(len > 0)
		len = 1.0f / len;
	
	x *= len;
	y *= len;
}

inline void Float2::Normalize( Float2& result ) const
{
	float len = Length();
	if(len > 0)
		len = 1.0f / len;
	
	result.x = x * len;
	result.y = y * len;
}

inline void Float2::Clamp( const Float2& vmin, const Float2& vmax )
{
	x = (x >= vmin.x) ? x : vmin.x;
	x = (x <= vmax.x) ? x : vmax.x;
	y = (y >= vmin.y) ? y : vmin.y;
	y = (y <= vmax.y) ? y : vmax.y;
}

inline void Float2::Clamp( const Float2& vmin, const Float2& vmax, Float2& result ) const
{
	float temp = x;
	temp = (temp >= vmin.x) ? temp : vmin.x;
	temp = (temp <= vmax.x) ? temp : vmax.x;
	result.x = temp;

	temp = y;
	temp = (y >= vmin.y) ? y : vmin.y;
	temp = (y <= vmax.y) ? y : vmax.y;
	result.y = temp;
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

inline float Float2::Distance( const Float2& v1, const Float2& v2 )
{
	Float2 v = v1 - v2;
	return v.Length();
}

inline float Float2::DistanceSquared( const Float2& v1, const Float2& v2 )
{
	Float2 v = v1 - v2;
	return v.LengthSquared();
}

inline void Float2::Min( const Float2& v1, const Float2& v2, Float2& result )
{
	result.x = (v1.x <= v2.x) ? v1.x : v2.x;
	result.y = (v1.y <= v2.y) ? v1.y : v2.y;
}

inline Float2 Float2::Min( const Float2& v1, const Float2& v2 )
{
    return Float2(
		(v1.x <= v2.x) ? v1.x : v2.x,
		(v1.y <= v2.y) ? v1.y : v2.y);
}

inline void Float2::Max( const Float2& v1, const Float2& v2, Float2& result )
{
	result.x = (v1.x >= v2.x) ? v1.x : v2.x;
	result.y = (v1.y >= v2.y) ? v1.y : v2.y;
}

inline Float2 Float2::Max( const Float2& v1, const Float2& v2 )
{
	return Float2(
		(v1.x >= v2.x) ? v1.x : v2.x,
		(v1.y >= v2.y) ? v1.y : v2.y);
}

inline void Float2::Lerp( const Float2& v1, const Float2& v2, float t, Float2& result )
{
	result.x = v1.x + ((v2.x - v1.x) * t);
	result.y = v1.y + ((v2.y - v1.y) * t);

}

inline Float2 Float2::Lerp( const Float2& v1, const Float2& v2, float t )
{
	return Float2(
		v1.x + ((v2.x - v1.x) * t),
		v1.y + ((v2.y - v1.y) * t));
}

inline void Float2::SmoothStep( const Float2& v1, const Float2& v2, float t, Float2& result )
{
    t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
    t = t*t*(3.f - 2.f*t);
	result.x = v1.x + ((v2.x - v1.x) * t);
	result.y = v1.y + ((v2.y - v1.y) * t);
}

inline Float2 Float2::SmoothStep( const Float2& v1, const Float2& v2, float t )
{
	t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
	t = t*t*(3.f - 2.f*t);	
    return Float2(
		v1.x + ((v2.x - v1.x) * t),
		v1.y + ((v2.y - v1.y) * t));
}

inline void Float2::Barycentric( const Float2& v1, const Float2& v2, const Float2& v3, float f, float g, Float2& result )
{
	result.x = (v1.x + (f * (v2.x - v1.x))) + (g * (v3.x - v1.x));
	result.y = (v1.y + (f * (v2.y - v1.y))) + (g * (v3.y - v1.y));
}

inline Float2 Float2::Barycentric( const Float2& v1, const Float2& v2, const Float2& v3, float f, float g )
{
    return Float2(
		(v1.x + (f * (v2.x - v1.x))) + (g * (v3.x - v1.x)),
		(v1.y + (f * (v2.y - v1.y))) + (g * (v3.y - v1.y)));
}	 

inline void Float2::CatmullRom( const Float2& v1, const Float2& v2, const Float2& v3, const Float2& v4, float t, Float2& result )
{
#if defined(_XM_NO_INTRINSICS_)

	float t2 = t * t;
	float t3 = t * t2;
	result.x = 0.5f * ((((2.0f * v2.x) + ((-v1.x + v3.x) * t)) + (((((2.0f * v1.x) - (5.0f * v2.x)) + (4.0f * v3.x)) - v4.x) * t2)) + ((((-v1.x + (3.0f * v2.x)) - (3.0f * v3.x)) + v4.x) * t3));
	result.y = 0.5f * ((((2.0f * v2.y) + ((-v1.y + v3.y) * t)) + (((((2.0f * v1.y) - (5.0f * v2.y)) + (4.0f * v3.y)) - v4.y) * t2)) + ((((-v1.y + (3.0f * v2.y)) - (3.0f * v3.y)) + v4.y) * t3));

#else
	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat2( &v1 );
	XMVECTOR x2 = XMLoadFloat2( &v2 );
	XMVECTOR x3 = XMLoadFloat2( &v3 );
	XMVECTOR x4 = XMLoadFloat2( &v4 );
	XMVECTOR X = XMVectorCatmullRom( x1, x2, x3, x4, t );
	XMStoreFloat2( &result, X );
#endif
}

inline Float2 Float2::CatmullRom( const Float2& v1, const Float2& v2, const Float2& v3, const Float2& v4, float t )
{
#if defined(_XM_NO_INTRINSICS_)

	float t2 = t * t;
	float t3 = t * t2;
	Float2 result;
	result.x = 0.5f * ((((2.0f * v2.x) + ((-v1.x + v3.x) * t)) + (((((2.0f * v1.x) - (5.0f * v2.x)) + (4.0f * v3.x)) - v4.x) * t2)) + ((((-v1.x + (3.0f * v2.x)) - (3.0f * v3.x)) + v4.x) * t3));
	result.y = 0.5f * ((((2.0f * v2.y) + ((-v1.y + v3.y) * t)) + (((((2.0f * v1.y) - (5.0f * v2.y)) + (4.0f * v3.y)) - v4.y) * t2)) + ((((-v1.y + (3.0f * v2.y)) - (3.0f * v3.y)) + v4.y) * t3));
	return result;
#else

	using namespace DirectX;
	XMVECTOR x1 = XMLoadFloat2( &v1 );
	XMVECTOR x2 = XMLoadFloat2( &v2 );
	XMVECTOR x3 = XMLoadFloat2( &v3 );
	XMVECTOR x4 = XMLoadFloat2( &v4 );
	XMVECTOR X = XMVectorCatmullRom( x1, x2, x3, x4, t );
    Float2 result;
    XMStoreFloat2( &result, X );
    return result;

#endif
}

inline void Float2::Hermite( const Float2& pos1, const Float2& tan1, const Float2& pos2, const Float2& tan2, float t, Float2& result )
{
	float t2 = t * t;
	float t3 = t * t2;
	float p0 = ((2.0f * t3) - (3.0f * t2)) + 1.0f;
	float p1 = (-2.0f * t3) + (3.0f * t2);
	float t0 = (t3 - (2.0f * t2)) + t;
	float t1 = t3 - t2;
	result.x = (((pos1.x * p0) + (pos2.x * p1)) + (tan1.x * t0)) + (tan2.x * t1);
	result.y = (((pos1.y * p0) + (pos2.y * p1)) + (tan1.y * t0)) + (tan2.y * t1);
}

inline Float2 Float2::Hermite( const Float2& pos1, const Float2& tan1, const Float2& pos2, const Float2& tan2, float t )
{
	float t2 = t * t;
	float t3 = t * t2;
	float p0 = ((2.0f * t3) - (3.0f * t2)) + 1.0f;
	float p1 = (-2.0f * t3) + (3.0f * t2);
	float t0 = (t3 - (2.0f * t2)) + t;
	float t1 = t3 - t2;
	Float2 result;
	result.x = (((pos1.x * p0) + (pos2.x * p1)) + (tan1.x * t0)) + (tan2.x * t1);
	result.y = (((pos1.y * p0) + (pos2.y * p1)) + (tan1.y * t0)) + (tan2.y * t1);
}

inline void Float2::Reflect( const Float2& ivec, const Float2& nvec, Float2& result )
{
	float t = (ivec.x * nvec.x) + (ivec.y * nvec.y);
	t *= 2.0f;
	result.x = ivec.x - (t * nvec.x);
	result.y = ivec.y - (t * nvec.y);
}

inline Float2 Float2::Reflect( const Float2& ivec, const Float2& nvec )
{
	float t = (ivec.x * nvec.x) + (ivec.y * nvec.y);
	t *= 2.0f;
	Float2(
		ivec.x - (t * nvec.x),
		ivec.y - (t * nvec.y));
}

inline void Float2::Refract( const Float2& ivec, const Float2& nvec, float refractionIndex, Float2& result )
{
    using namespace DirectX;
    XMVECTOR i = XMLoadFloat2( &ivec );
    XMVECTOR n = XMLoadFloat2( &nvec );
    XMVECTOR X = XMVector2Refract( i, n, refractionIndex );
    XMStoreFloat2( &result, X );
}

inline Float2 Float2::Refract( const Float2& ivec, const Float2& nvec, float refractionIndex )
{
    using namespace DirectX;
    XMVECTOR i = XMLoadFloat2( &ivec );
    XMVECTOR n = XMLoadFloat2( &nvec );
    XMVECTOR X = XMVector2Refract( i, n, refractionIndex );

    Float2 result;
    XMStoreFloat2( &result, X );
    return result;
}

inline void Float2::Transform( const Float2& v, const Quaternion& quat, Float2& result )
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2( &v );
    XMVECTOR q = XMLoadFloat4( &quat );
    XMVECTOR X = XMVector3Rotate( v1, q );
    XMStoreFloat2( &result, X );
}

inline Float2 Float2::Transform( const Float2& v, const Quaternion& quat )
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2( &v );
    XMVECTOR q = XMLoadFloat4( &quat );
    XMVECTOR X = XMVector3Rotate( v1, q );

    Float2 result;
    XMStoreFloat2( &result, X );
    return result;
}

inline void Float2::Transform( const Float2& v, const Float4x4& m, Float2& result )
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2( &v );
    XMMATRIX M = XMLoadFloat4x4( &m );
    XMVECTOR X = XMVector2TransformCoord( v1, M );
    XMStoreFloat2( &result, X );
}

inline Float2 Float2::Transform( const Float2& v, const Float4x4& m )
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2( &v );
    XMMATRIX M = XMLoadFloat4x4( &m );
    XMVECTOR X = XMVector2TransformCoord( v1, M );

    Float2 result;
    XMStoreFloat2( &result, X );
    return result;
}

_Use_decl_annotations_
inline void Float2::Transform( const Float2* varray, size_t count, const Float4x4& m, Float2* resultArray )
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4( &m );
    XMVector2TransformCoordStream( resultArray, sizeof(XMFLOAT2), varray, sizeof(XMFLOAT2), count, M );
}

inline void Float2::Transform( const Float2& v, const Float4x4& m, Float4& result )
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2( &v );
    XMMATRIX M = XMLoadFloat4x4( &m );
    XMVECTOR X = XMVector2Transform( v1, M );
    XMStoreFloat4( &result, X );
}

_Use_decl_annotations_
inline void Float2::Transform( const Float2* varray, size_t count, const Float4x4& m, Float4* resultArray )
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4( &m );
    XMVector2TransformStream( resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT2), count, M );
}

inline void Float2::TransformNormal( const Float2& v, const Float4x4& m, Float2& result )
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2( &v );
    XMMATRIX M = XMLoadFloat4x4( &m );
    XMVECTOR X = XMVector2TransformNormal( v1, M );
    XMStoreFloat2( &result, X );
}

inline Float2 Float2::TransformNormal( const Float2& v, const Float4x4& m )
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2( &v );
    XMMATRIX M = XMLoadFloat4x4( &m );
    XMVECTOR X = XMVector2TransformNormal( v1, M );

    Float2 result;
    XMStoreFloat2( &result, X );
    return result;
}

_Use_decl_annotations_
inline void Float2::TransformNormal( const Float2* varray, size_t count, const Float4x4& m, Float2* resultArray )
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4( &m );
    XMVector2TransformNormalStream( resultArray, sizeof(XMFLOAT2), varray, sizeof(XMFLOAT2), count, M );
}
#pragma endregion 

#pragma region Float3
/****************************************************************************
 *
 * Float3
 *
 ****************************************************************************/
inline bool Float3::operator == ( const Float3& V ) const
{
	return (x == V.x) && (y==V.y) && (z == V.z);
}

inline bool Float3::operator != ( const Float3& V ) const
{
    return (x != V.x) || (y != V.y) || (z != V.z);
}

inline Float3& Float3::operator+= (const Float3& V)
{
    x += V.x;
	y += V.y;
	z += V.z;
    return *this;
}

inline Float3& Float3::operator-= (const Float3& V)
{
	x -= V.x;
	y -= V.y;
	z -= V.z;
	return *this;
}

inline Float3& Float3::operator*= (const Float3& V)
{
	x *= V.x;
	y *= V.y;
	z *= V.z;
	return *this;
}

inline Float3& Float3::operator*= (float S)
{
	x *= S;
	y *= S;
	z *= S;
	return *this;
} 

inline Float3& Float3::operator/= (float S)
{
    assert( S != 0.0f );
	S = 1 / S;
	x *= S;
	y *= S;
	z *= S;
	return *this;
} 

inline Float3 Float3::operator- () const
{
	return Float3(-x,-y,-z);
}

inline Float3 operator+ (const Float3& V1, const Float3& V2)
{
	return Float3(V1.x + V2.x,V1.y + V2.y,V1.z + V2.z);
}

inline Float3 operator- (const Float3& V1, const Float3& V2)
{
   return Float3(V1.x - V2.x,V1.y - V2.y,V1.z - V2.z);
}

inline Float3 operator* (const Float3& V1, const Float3& V2)
{
    return Float3(V1.x * V2.x,V1.y * V2.y,V1.z * V2.z);
}

inline Float3 operator* (const Float3& V, float S)
{
	return Float3(V.x * S,V.y * S,V.z * S);
}

inline Float3 operator/ (const Float3& V1, const Float3& V2)
{
    return Float3(V1.x / V2.x,V1.y / V2.y,V1.z / V2.z);
}

inline Float3 operator* (float S, const Float3& V)
{
	return Float3(V.x * S,V.y * S,V.z * S);
}

//------------------------------------------------------------------------------
// Vector operations
//------------------------------------------------------------------------------

inline bool Float3::InBounds( const Float3& Bounds ) const
{
	return ((x <= Bounds.x) && (x>=-Bounds.x)
		&& (y <= Bounds.y) && (y >= -Bounds.y)
		&& (z <= Bounds.z) && (z >= -Bounds.z));
}

inline float Float3::Length() const
{
   return sqrt(x*x + y*y + z*z);
}

inline float Float3::LengthSquared() const
{
    return (x*x + y*y + z*z);
}

inline float Float3::Dot( const Float3& V ) const
{
   return (x * V.x + y * V.y + z * V.z);
}

inline void Float3::Cross( const Float3& V, Float3& result ) const
{
	result.x = (y * V.z) - (z * V.y);
	result.y = (z * V.x) - (x * V.z);
	result.z = (x * V.y) - (y * V.x);
}

inline Float3 Float3::Cross( const Float3& V ) const
{
	Float3 result;
	result.x = (y * V.z) - (z * V.y);
	result.y = (z * V.x) - (x * V.z);
	result.z = (x * V.y) - (y * V.x);
}

inline void Float3::Normalize()
{
	float length = Length();
	if(length > 0)
		length = 1.0f / length;

	x *= length;
	y *= length;
	z *= length;
}

inline void Float3::Normalize( Float3& result ) const
{
	float length = Length();
	if(length > 0)
		length = 1.0f / length;

	result.x = x * length;
	result.y = y * length;
	result.z = z * length;
}

inline void Float3::Clamp( const Float3& vmin, const Float3& vmax )
{
	x = (x >= vmin.x) ? x : vmin.x;
	x = (x <= vmax.x) ? x :	vmax.x;
	y = (y >= vmin.y) ? y : vmin.y;
	y = (y <= vmax.y) ? y :	vmax.y;
	z = (z >= vmin.z) ? z : vmin.z;
	z = (z <= vmax.z) ? z :	vmax.z;
}

inline void Float3::Clamp( const Float3& vmin, const Float3& vmax, Float3& result ) const
{
	result.x = (x >= vmin.x) ? x : vmin.x;
	result.x = (x <= vmax.x) ? x : vmax.x;
	result.y = (y >= vmin.y) ? y : vmin.y;
	result.y = (y <= vmax.y) ? y : vmax.y;
	result.z = (z >= vmin.z) ? z : vmin.z;
	result.z = (z <= vmax.z) ? z : vmax.z;
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

inline float Float3::Distance( const Float3& v1, const Float3& v2 )
{
	float deltaX = v1.x - v2.x;
	float deltaY = v1.y - v2.y;
	float deltaZ = v1.z - v2.z;
	return sqrt(deltaX*deltaX + deltaY*deltaY + deltaZ * deltaZ);
}

inline float Float3::DistanceSquared( const Float3& v1, const Float3& v2 )
{
	float deltaX = v1.x - v2.x;
	float deltaY = v1.y - v2.y;
	float deltaZ = v1.z - v2.z;
	return (deltaX*deltaX + deltaY*deltaY + deltaZ * deltaZ);
}

inline void Float3::Min( const Float3& v1, const Float3& v2, Float3& result )
{
   result.x = (v1.x <= v2.x) ? v1.x : v2.x;
   result.y = (v1.y <= v2.y) ? v1.y : v2.y;
   result.z = (v1.z <= v2.z) ? v1.z : v2.z;
}

inline Float3 Float3::Min( const Float3& v1, const Float3& v2 )
{
    Float3 result;
	result.x = (v1.x <= v2.x) ? v1.x : v2.x;
	result.y = (v1.y <= v2.y) ? v1.y : v2.y;
	result.z = (v1.z <= v2.z) ? v1.z : v2.z;
	return result;
}

inline void Float3::Max( const Float3& v1, const Float3& v2, Float3& result )
{
	result.x = (v1.x >= v2.x) ? v1.x : v2.x;
	result.y = (v1.y >= v2.y) ? v1.y : v2.y;
	result.z = (v1.z >= v2.z) ? v1.z : v2.z;
}

inline Float3 Float3::Max( const Float3& v1, const Float3& v2 )
{
	Float3 result;
	result.x = (v1.x >= v2.x) ? v1.x : v2.x;
	result.y = (v1.y >= v2.y) ? v1.y : v2.y;
	result.z = (v1.z >= v2.z) ? v1.z : v2.z;
	return result;
}

inline void Float3::Lerp( const Float3& v1, const Float3& v2, float t, Float3& result )
{
	result.x = v1.x + ((v2.x - v1.x) * t);
	result.y = v1.y + ((v2.y - v1.y) * t);
	result.z = v1.z + ((v2.z - v1.z) * t);
}

inline Float3 Float3::Lerp( const Float3& v1, const Float3& v2, float t )
{
    Float3 result;
	result.x = v1.x + ((v2.x - v1.x) * t);
	result.y = v1.y + ((v2.y - v1.y) * t);
	result.z = v1.z + ((v2.z - v1.z) * t);
}

inline void Float3::SmoothStep( const Float3& v1, const Float3& v2, float t, Float3& result )
{
    t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
    t = t*t*(3.f - 2.f*t);
	result.x = v1.x + ((v2.x - v1.x) * t);
	result.y = v1.y + ((v2.y - v1.y) * t);
	result.z = v1.z + ((v2.z - v1.z) * t);
}

inline Float3 Float3::SmoothStep( const Float3& v1, const Float3& v2, float t )
{
	Float3 result;
	t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
	t = t*t*(3.f - 2.f*t);
	result.x = v1.x + ((v2.x - v1.x) * t);
	result.y = v1.y + ((v2.y - v1.y) * t);
	result.z = v1.z + ((v2.z - v1.z) * t);
}

inline void Float3::Barycentric( const Float3& v1, const Float3& v2, const Float3& v3, float f, float g, Float3& result)
{
	result.x = (v1.x + (f * (v2.x - v1.x))) + (g * (v3.x - v1.x));
	result.y = (v1.y + (f * (v2.y - v1.y))) + (g * (v3.y - v1.y));
	result.z = (v1.z + (f * (v2.z - v1.z))) + (g * (v3.z - v1.z));
}

inline Float3 Float3::Barycentric( const Float3& v1, const Float3& v2, const Float3& v3, float f, float g )
{
    Float3 result;
	result.x = (v1.x + (f * (v2.x - v1.x))) + (g * (v3.x - v1.x));
	result.y = (v1.y + (f * (v2.y - v1.y))) + (g * (v3.y - v1.y));
	result.z = (v1.z + (f * (v2.z - v1.z))) + (g * (v3.z - v1.z));
	return result;
}

inline void Float3::CatmullRom( const Float3& v1, const Float3& v2, const Float3& v3, const Float3& v4, float t, Float3& result )
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat3( &v1 );
    XMVECTOR x2 = XMLoadFloat3( &v2 );
    XMVECTOR x3 = XMLoadFloat3( &v3 );
    XMVECTOR x4 = XMLoadFloat3( &v4 );
    XMVECTOR X = XMVectorCatmullRom( x1, x2, x3, x4, t );
    XMStoreFloat3( &result, X );
}

inline Float3 Float3::CatmullRom( const Float3& v1, const Float3& v2, const Float3& v3, const Float3& v4, float t )
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat3( &v1 );
    XMVECTOR x2 = XMLoadFloat3( &v2 );
    XMVECTOR x3 = XMLoadFloat3( &v3 );
    XMVECTOR x4 = XMLoadFloat3( &v4 );
    XMVECTOR X = XMVectorCatmullRom( x1, x2, x3, x4, t );

    Float3 result;
    XMStoreFloat3( &result, X );
    return result;
}

inline void Float3::Hermite( const Float3& v1, const Float3& t1, const Float3& v2, const Float3& t2, float t, Float3& result )
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat3( &v1 );
    XMVECTOR x2 = XMLoadFloat3( &t1 );
    XMVECTOR x3 = XMLoadFloat3( &v2 );
    XMVECTOR x4 = XMLoadFloat3( &t2 );
    XMVECTOR X = XMVectorHermite( x1, x2, x3, x4, t );
    XMStoreFloat3( &result, X );
}

inline Float3 Float3::Hermite( const Float3& v1, const Float3& t1, const Float3& v2, const Float3& t2, float t )
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat3( &v1 );
    XMVECTOR x2 = XMLoadFloat3( &t1 );
    XMVECTOR x3 = XMLoadFloat3( &v2 );
    XMVECTOR x4 = XMLoadFloat3( &t2 );
    XMVECTOR X = XMVectorHermite( x1, x2, x3, x4, t );

    Float3 result;
    XMStoreFloat3( &result, X );
    return result;
}

inline void Float3::Reflect( const Float3& ivec, const Float3& nvec, Float3& result )
{
    using namespace DirectX;
    XMVECTOR i = XMLoadFloat3( &ivec );
    XMVECTOR n = XMLoadFloat3( &nvec );
    XMVECTOR X = XMVector3Reflect( i, n );
    XMStoreFloat3( &result, X );
}

inline Float3 Float3::Reflect( const Float3& ivec, const Float3& nvec )
{
    using namespace DirectX;
    XMVECTOR i = XMLoadFloat3( &ivec );
    XMVECTOR n = XMLoadFloat3( &nvec );
    XMVECTOR X = XMVector3Reflect( i, n );

    Float3 result;
    XMStoreFloat3( &result, X );
    return result;
}

inline void Float3::Refract( const Float3& ivec, const Float3& nvec, float refractionIndex, Float3& result )
{
    using namespace DirectX;
    XMVECTOR i = XMLoadFloat3( &ivec );
    XMVECTOR n = XMLoadFloat3( &nvec );
    XMVECTOR X = XMVector3Refract( i, n, refractionIndex );
    XMStoreFloat3( &result, X );
}

inline Float3 Float3::Refract( const Float3& ivec, const Float3& nvec, float refractionIndex )
{
    using namespace DirectX;
    XMVECTOR i = XMLoadFloat3( &ivec );
    XMVECTOR n = XMLoadFloat3( &nvec );
    XMVECTOR X = XMVector3Refract( i, n, refractionIndex );

    Float3 result;
    XMStoreFloat3( &result, X );
    return result;
}

inline void Float3::Transform( const Float3& v, const Quaternion& quat, Float3& result )
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3( &v );
    XMVECTOR q = XMLoadFloat4( &quat );
    XMVECTOR X = XMVector3Rotate( v1, q );
    XMStoreFloat3( &result, X );
}

inline Float3 Float3::Transform( const Float3& v, const Quaternion& quat )
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3( &v );
    XMVECTOR q = XMLoadFloat4( &quat );
    XMVECTOR X = XMVector3Rotate( v1, q );

    Float3 result;
    XMStoreFloat3( &result, X );
    return result;
}

inline void Float3::Transform( const Float3& v, const Float4x4& m, Float3& result )
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3( &v );
    XMMATRIX M = XMLoadFloat4x4( &m );
    XMVECTOR X = XMVector3TransformCoord( v1, M );
    XMStoreFloat3( &result, X );
}

inline Float3 Float3::Transform( const Float3& v, const Float4x4& m )
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3( &v );
    XMMATRIX M = XMLoadFloat4x4( &m );
    XMVECTOR X = XMVector3TransformCoord( v1, M );

    Float3 result;
    XMStoreFloat3( &result, X );
    return result;
}

_Use_decl_annotations_
inline void Float3::Transform( const Float3* varray, size_t count, const Float4x4& m, Float3* resultArray )
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4( &m );
    XMVector3TransformCoordStream( resultArray, sizeof(XMFLOAT3), varray, sizeof(XMFLOAT3), count, M );
}

inline void Float3::Transform( const Float3& v, const Float4x4& m, Float4& result )
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3( &v );
    XMMATRIX M = XMLoadFloat4x4( &m );
    XMVECTOR X = XMVector3Transform( v1, M );
    XMStoreFloat4( &result, X );
}

_Use_decl_annotations_
inline void Float3::Transform( const Float3* varray, size_t count, const Float4x4& m, Float4* resultArray )
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4( &m );
    XMVector3TransformStream( resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT3), count, M );
}

inline void Float3::TransformNormal( const Float3& v, const Float4x4& m, Float3& result )
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3( &v );
    XMMATRIX M = XMLoadFloat4x4( &m );
    XMVECTOR X = XMVector3TransformNormal( v1, M );
    XMStoreFloat3( &result, X );
}

inline Float3 Float3::TransformNormal( const Float3& v, const Float4x4& m )
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3( &v );
    XMMATRIX M = XMLoadFloat4x4( &m );
    XMVECTOR X = XMVector3TransformNormal( v1, M );

    Float3 result;
    XMStoreFloat3( &result, X );
    return result;
}

_Use_decl_annotations_
inline void Float3::TransformNormal( const Float3* varray, size_t count, const Float4x4& m, Float3* resultArray )
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4( &m );
    XMVector3TransformNormalStream( resultArray, sizeof(XMFLOAT3), varray, sizeof(XMFLOAT3), count, M );
}

#pragma endregion

#pragma region Float4
/****************************************************************************
 *
 * Float4
 *
 ****************************************************************************/
inline bool Float4::operator == ( const Float4& V ) const
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4( this );
    XMVECTOR v2 = XMLoadFloat4( &V );
    return XMVector4Equal( v1, v2 );
}

inline bool Float4::operator != ( const Float4& V ) const
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4( this );
    XMVECTOR v2 = XMLoadFloat4( &V );
    return XMVector4NotEqual( v1, v2 );
}

inline Float4& Float4::operator+= (const Float4& V)
{
	x += V.x;
	y += V.y;
	z += V.z;
	w += V.w;
	return *this;
}

inline Float4& Float4::operator-= (const Float4& V)
{
	x -= V.x;
	y -= V.y;
	z -= V.z;
	w -= V.w;
    return *this;
}

inline Float4& Float4::operator*= (const Float4& V)
{
	x *= V.x;
	y *= V.y;
	z *= V.z;
	w *= V.w;
	return *this;
}

inline Float4& Float4::operator*= (float S)
{
	x *= S;
	y *= S;
	z *= S;
	w *= S;
	return *this;
} 

inline Float4& Float4::operator/= (float S)
{
	x /= S;
	y /= S;
	z /= S;
	w /= S;
	return *this;
} 

//------------------------------------------------------------------------------
// Urnary operators
//------------------------------------------------------------------------------

inline Float4 Float4::operator- () const
{
    return Float4(-x,-y,-z,-w);
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

inline Float4 operator+ (const Float4& V1, const Float4& V2)
{
	return Float4(V1.x + V2.x,V1.y+V2.y,V1.z+V2.z,V1.w+V2.w);
}

inline Float4 operator- (const Float4& V1, const Float4& V2)
{
	return Float4(V1.x - V2.x,V1.y-V2.y,V1.z-V2.z,V1.w-V2.w);
}

inline Float4 operator* (const Float4& V1, const Float4& V2)
{
   return Float4(V1.x * V2.x,V1.y*V2.y,V1.z*V2.z,V1.w*V2.w);
}

inline Float4 operator* (const Float4& V, float S)
{
  return Float4(V.x * S,V.y*S,V.z*S,V.w*S);
}

inline Float4 operator/ (const Float4& V1, const Float4& V2)
{
   return Float4(V1.x / V2.x,V1.y / V2.y, V1.z / V2.z, V1.w / V2.w);
}

inline Float4 operator* (float S, const Float4& V)
{
   return Float4(V.x * S, V.y*S, V.z*S, V.w*S);
}

//------------------------------------------------------------------------------
// Vector operations
//------------------------------------------------------------------------------

inline bool Float4::InBounds( const Float4& Bounds ) const
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4( this );
    XMVECTOR v2 = XMLoadFloat4( &Bounds );
    return XMVector4InBounds( v1, v2 );
}

inline float Float4::Length() const
{
	return sqrt(x*x+y*y+z*z+w*w);
}

inline float Float4::LengthSquared() const
{
    return (x*x+y*y+z*z+w*w);
}

inline float Float4::Dot( const Float4& V ) const
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4( this );
    XMVECTOR v2 = XMLoadFloat4( &V );
    XMVECTOR X = XMVector4Dot( v1, v2 );
    return XMVectorGetX( X );
}

inline void Float4::Cross( const Float4& v1, const Float4& v2, Float4& result ) const
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( this );
    XMVECTOR x2 = XMLoadFloat4( &v1 );
    XMVECTOR x3 = XMLoadFloat4( &v2 );
    XMVECTOR R = XMVector4Cross( x1, x2, x3 );
    XMStoreFloat4( &result, R );
}

inline Float4 Float4::Cross( const Float4& v1, const Float4& v2 ) const
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( this );
    XMVECTOR x2 = XMLoadFloat4( &v1 );
    XMVECTOR x3 = XMLoadFloat4( &v2 );
    XMVECTOR R = XMVector4Cross( x1, x2, x3 );

    Float4 result;
    XMStoreFloat4( &result, R );
    return result;
}

inline void Float4::Normalize()
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4( this );
    XMVECTOR X = XMVector4Normalize( v1 );
    XMStoreFloat4( this, X );
}

inline void Float4::Normalize( Float4& result ) const
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4( this );
    XMVECTOR X = XMVector4Normalize( v1 );
    XMStoreFloat4( &result, X );
}

inline void Float4::Clamp( const Float4& vmin, const Float4& vmax )
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4( this );
    XMVECTOR v2 = XMLoadFloat4( &vmin );
    XMVECTOR v3 = XMLoadFloat4( &vmax );
    XMVECTOR X = XMVectorClamp( v1, v2, v3 );
    XMStoreFloat4( this, X );
}

inline void Float4::Clamp( const Float4& vmin, const Float4& vmax, Float4& result ) const
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4( this );
    XMVECTOR v2 = XMLoadFloat4( &vmin );
    XMVECTOR v3 = XMLoadFloat4( &vmax );
    XMVECTOR X = XMVectorClamp( v1, v2, v3 );
    XMStoreFloat4( &result, X );
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

inline float Float4::Distance( const Float4& v1, const Float4& v2 )
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( &v1 );
    XMVECTOR x2 = XMLoadFloat4( &v2 );
    XMVECTOR V = XMVectorSubtract( x2, x1 );
    XMVECTOR X = XMVector4Length( V );
    return XMVectorGetX( X );
}

inline float Float4::DistanceSquared( const Float4& v1, const Float4& v2 )
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( &v1 );
    XMVECTOR x2 = XMLoadFloat4( &v2 );
    XMVECTOR V = XMVectorSubtract( x2, x1 );
    XMVECTOR X = XMVector4LengthSq( V );
    return XMVectorGetX( X );
}

inline void Float4::Min( const Float4& v1, const Float4& v2, Float4& result )
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( &v1 );
    XMVECTOR x2 = XMLoadFloat4( &v2 );
    XMVECTOR X = XMVectorMin( x1, x2 );
    XMStoreFloat4( &result, X );
}

inline Float4 Float4::Min( const Float4& v1, const Float4& v2 )
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( &v1 );
    XMVECTOR x2 = XMLoadFloat4( &v2 );
    XMVECTOR X = XMVectorMin( x1, x2 );

    Float4 result;
    XMStoreFloat4( &result, X );
    return result;
}

inline void Float4::Max( const Float4& v1, const Float4& v2, Float4& result )
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( &v1 );
    XMVECTOR x2 = XMLoadFloat4( &v2 );
    XMVECTOR X = XMVectorMax( x1, x2 );
    XMStoreFloat4( &result, X );
}

inline Float4 Float4::Max( const Float4& v1, const Float4& v2 )
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( &v1 );
    XMVECTOR x2 = XMLoadFloat4( &v2 );
    XMVECTOR X = XMVectorMax( x1, x2 );

    Float4 result;
    XMStoreFloat4( &result, X );
    return result;
}

inline void Float4::Lerp( const Float4& v1, const Float4& v2, float t, Float4& result )
{
	result.x = v1.x + ((v2.x - v1.x) * t);
	result.y = v1.y + ((v2.y - v1.y) * t);
	result.z = v1.z + ((v2.z - v1.z) * t);
	result.w = v1.w + ((v2.w - v1.w) * t);
}

inline Float4 Float4::Lerp( const Float4& v1, const Float4& v2, float t )
{	
	Float4 result;
	result.x = v1.x + ((v2.x - v1.x) * t);
	result.y = v1.y + ((v2.y - v1.y) * t);
	result.z = v1.z + ((v2.z - v1.z) * t);
	result.w = v1.w + ((v2.w - v1.w) * t);
	return result;
}

inline void Float4::SmoothStep( const Float4& v1, const Float4& v2, float t, Float4& result )
{
    using namespace DirectX;
    t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
    t = t*t*(3.f - 2.f*t);
	result.x = v1.x + ((v2.x - v1.x) * t);
	result.y = v1.y + ((v2.y - v1.y) * t);
	result.z = v1.z + ((v2.z - v1.z) * t);
	result.w = v1.w + ((v2.w - v1.w) * t);
}

inline Float4 Float4::SmoothStep( const Float4& v1, const Float4& v2, float t )
{
    using namespace DirectX;
    t = (t > 1.0f) ? 1.0f : ((t < 0.0f) ? 0.0f : t);  // Clamp value to 0 to 1
    t = t*t*(3.f - 2.f*t);
	Float4 result;
	result.x = v1.x + ((v2.x - v1.x) * t);
	result.y = v1.y + ((v2.y - v1.y) * t);
	result.z = v1.z + ((v2.z - v1.z) * t);
	result.w = v1.w + ((v2.w - v1.w) * t);
	return result;
}

inline void Float4::Barycentric( const Float4& v1, const Float4& v2, const Float4& v3, float f, float g, Float4& result )
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( &v1 );
    XMVECTOR x2 = XMLoadFloat4( &v2 );
    XMVECTOR x3 = XMLoadFloat4( &v3 );
    XMVECTOR X = XMVectorBaryCentric( x1, x2, x3, f, g );
    XMStoreFloat4( &result, X );
}

inline Float4 Float4::Barycentric( const Float4& v1, const Float4& v2, const Float4& v3, float f, float g )
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( &v1 );
    XMVECTOR x2 = XMLoadFloat4( &v2 );
    XMVECTOR x3 = XMLoadFloat4( &v3 );
    XMVECTOR X = XMVectorBaryCentric( x1, x2, x3, f, g );

    Float4 result;
    XMStoreFloat4( &result, X );
    return result;
}

inline void Float4::CatmullRom( const Float4& v1, const Float4& v2, const Float4& v3, const Float4& v4, float t, Float4& result )
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( &v1 );
    XMVECTOR x2 = XMLoadFloat4( &v2 );
    XMVECTOR x3 = XMLoadFloat4( &v3 );
    XMVECTOR x4 = XMLoadFloat4( &v4 );
    XMVECTOR X = XMVectorCatmullRom( x1, x2, x3, x4, t );
    XMStoreFloat4( &result, X );
}

inline Float4 Float4::CatmullRom( const Float4& v1, const Float4& v2, const Float4& v3, const Float4& v4, float t )
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( &v1 );
    XMVECTOR x2 = XMLoadFloat4( &v2 );
    XMVECTOR x3 = XMLoadFloat4( &v3 );
    XMVECTOR x4 = XMLoadFloat4( &v4 );
    XMVECTOR X = XMVectorCatmullRom( x1, x2, x3, x4, t );

    Float4 result;
    XMStoreFloat4( &result, X );
    return result;
}

inline void Float4::Hermite( const Float4& v1, const Float4& t1, const Float4& v2, const Float4& t2, float t, Float4& result )
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( &v1 );
    XMVECTOR x2 = XMLoadFloat4( &t1 );
    XMVECTOR x3 = XMLoadFloat4( &v2 );
    XMVECTOR x4 = XMLoadFloat4( &t2 );
    XMVECTOR X = XMVectorHermite( x1, x2, x3, x4, t );
    XMStoreFloat4( &result, X );
}

inline Float4 Float4::Hermite( const Float4& v1, const Float4& t1, const Float4& v2, const Float4& t2, float t )
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( &v1 );
    XMVECTOR x2 = XMLoadFloat4( &t1 );
    XMVECTOR x3 = XMLoadFloat4( &v2 );
    XMVECTOR x4 = XMLoadFloat4( &t2 );
    XMVECTOR X = XMVectorHermite( x1, x2, x3, x4, t );

    Float4 result;
    XMStoreFloat4( &result, X );
    return result;
}

inline void Float4::Reflect( const Float4& ivec, const Float4& nvec, Float4& result )
{
    using namespace DirectX;
    XMVECTOR i = XMLoadFloat4( &ivec );
    XMVECTOR n = XMLoadFloat4( &nvec );
    XMVECTOR X = XMVector4Reflect( i, n );
    XMStoreFloat4( &result, X );
}

inline Float4 Float4::Reflect( const Float4& ivec, const Float4& nvec )
{
    using namespace DirectX;
    XMVECTOR i = XMLoadFloat4( &ivec );
    XMVECTOR n = XMLoadFloat4( &nvec );
    XMVECTOR X = XMVector4Reflect( i, n );

    Float4 result;
    XMStoreFloat4( &result, X );
    return result;
}

inline void Float4::Refract( const Float4& ivec, const Float4& nvec, float refractionIndex, Float4& result )
{
    using namespace DirectX;
    XMVECTOR i = XMLoadFloat4( &ivec );
    XMVECTOR n = XMLoadFloat4( &nvec );
    XMVECTOR X = XMVector4Refract( i, n, refractionIndex );
    XMStoreFloat4( &result, X );
}

inline Float4 Float4::Refract( const Float4& ivec, const Float4& nvec, float refractionIndex )
{
    using namespace DirectX;
    XMVECTOR i = XMLoadFloat4( &ivec );
    XMVECTOR n = XMLoadFloat4( &nvec );
    XMVECTOR X = XMVector4Refract( i, n, refractionIndex );

    Float4 result;
    XMStoreFloat4( &result, X );
    return result;
}

inline void Float4::Transform( const Float2& v, const Quaternion& quat, Float4& result )
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2( &v );
    XMVECTOR q = XMLoadFloat4( &quat );
    XMVECTOR X = XMVector3Rotate( v1, q );
    X = XMVectorSelect( g_XMIdentityR3, X, g_XMSelect1110 ); // result.w = 1.f
    XMStoreFloat4( &result, X );
}

inline Float4 Float4::Transform( const Float2& v, const Quaternion& quat )
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat2( &v );
    XMVECTOR q = XMLoadFloat4( &quat );
    XMVECTOR X = XMVector3Rotate( v1, q );
    X = XMVectorSelect( g_XMIdentityR3, X, g_XMSelect1110 ); // result.w = 1.f

    Float4 result;
    XMStoreFloat4( &result, X );
    return result;
}

inline void Float4::Transform( const Float3& v, const Quaternion& quat, Float4& result )
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3( &v );
    XMVECTOR q = XMLoadFloat4( &quat );
    XMVECTOR X = XMVector3Rotate( v1, q );
    X = XMVectorSelect( g_XMIdentityR3, X, g_XMSelect1110 ); // result.w = 1.f
    XMStoreFloat4( &result, X );
}

inline Float4 Float4::Transform( const Float3& v, const Quaternion& quat )
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat3( &v );
    XMVECTOR q = XMLoadFloat4( &quat );
    XMVECTOR X = XMVector3Rotate( v1, q );
    X = XMVectorSelect( g_XMIdentityR3, X, g_XMSelect1110 ); // result.w = 1.f

    Float4 result;
    XMStoreFloat4( &result, X );
    return result;
}

inline void Float4::Transform( const Float4& v, const Quaternion& quat, Float4& result )
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4( &v );
    XMVECTOR q = XMLoadFloat4( &quat );
    XMVECTOR X = XMVector3Rotate( v1, q );
    X = XMVectorSelect( v1, X, g_XMSelect1110 ); // result.w = v.w
    XMStoreFloat4( &result, X );
}

inline Float4 Float4::Transform( const Float4& v, const Quaternion& quat )
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4( &v );
    XMVECTOR q = XMLoadFloat4( &quat );
    XMVECTOR X = XMVector3Rotate( v1, q );
    X = XMVectorSelect( v1, X, g_XMSelect1110 ); // result.w = v.w

    Float4 result;
    XMStoreFloat4( &result, X );
    return result;
}

inline void Float4::Transform( const Float4& v, const Float4x4& m, Float4& result )
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4( &v );
    XMMATRIX M = XMLoadFloat4x4( &m );
    XMVECTOR X = XMVector4Transform( v1, M );
    XMStoreFloat4( &result, X );
}

inline Float4 Float4::Transform( const Float4& v, const Float4x4& m )
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4( &v );
    XMMATRIX M = XMLoadFloat4x4( &m );
    XMVECTOR X = XMVector4Transform( v1, M );

    Float4 result;
    XMStoreFloat4( &result, X );
    return result;
}

_Use_decl_annotations_
inline void Float4::Transform( const Float4* varray, size_t count, const Float4x4& m, Float4* resultArray )
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4( &m );
    XMVector4TransformStream( resultArray, sizeof(XMFLOAT4), varray, sizeof(XMFLOAT4), count, M );
}
#pragma endregion

#pragma region Matrix
/****************************************************************************
 *
 * Matrix
 *
 ****************************************************************************/
inline bool Float4x4::operator == ( const Float4x4& M ) const
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&_11) );
    XMVECTOR x2 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&_21) );
    XMVECTOR x3 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&_31) );
    XMVECTOR x4 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&_41) );

    XMVECTOR y1 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._11) );
    XMVECTOR y2 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._21) );
    XMVECTOR y3 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._31) );
    XMVECTOR y4 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._41) );

    return ( XMVector4Equal( x1, y1 )
             && XMVector4Equal( x2, y2 )
             && XMVector4Equal( x3, y3 )
             && XMVector4Equal( x4, y4 ) ) != 0;
}

inline bool Float4x4::operator != ( const Float4x4& M ) const
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&_11) );
    XMVECTOR x2 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&_21) );
    XMVECTOR x3 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&_31) );
    XMVECTOR x4 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&_41) );

    XMVECTOR y1 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._11) );
    XMVECTOR y2 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._21) );
    XMVECTOR y3 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._31) );
    XMVECTOR y4 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._41) );

    return ( XMVector4NotEqual( x1, y1 )
             && XMVector4NotEqual( x2, y2 )
             && XMVector4NotEqual( x3, y3 )
             && XMVector4NotEqual( x4, y4 ) ) != 0;
}

//------------------------------------------------------------------------------
// Assignment operators
//------------------------------------------------------------------------------

inline Float4x4& Float4x4::operator+= (const Float4x4& M)
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( reinterpret_cast<XMFLOAT4*>(&_11) );
    XMVECTOR x2 = XMLoadFloat4( reinterpret_cast<XMFLOAT4*>(&_21) );
    XMVECTOR x3 = XMLoadFloat4( reinterpret_cast<XMFLOAT4*>(&_31) );
    XMVECTOR x4 = XMLoadFloat4( reinterpret_cast<XMFLOAT4*>(&_41) );

    XMVECTOR y1 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._11) );
    XMVECTOR y2 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._21) );
    XMVECTOR y3 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._31) );
    XMVECTOR y4 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._41) );

    x1 = XMVectorAdd( x1, y1 );
    x2 = XMVectorAdd( x2, y2 );
    x3 = XMVectorAdd( x3, y3 );
    x4 = XMVectorAdd( x4, y4 );

    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&_11), x1 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&_21), x2 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&_31), x3 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&_41), x4 );
    return *this;
}

inline Float4x4& Float4x4::operator-= (const Float4x4& M)
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( reinterpret_cast<XMFLOAT4*>(&_11) );
    XMVECTOR x2 = XMLoadFloat4( reinterpret_cast<XMFLOAT4*>(&_21) );
    XMVECTOR x3 = XMLoadFloat4( reinterpret_cast<XMFLOAT4*>(&_31) );
    XMVECTOR x4 = XMLoadFloat4( reinterpret_cast<XMFLOAT4*>(&_41) );

    XMVECTOR y1 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._11) );
    XMVECTOR y2 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._21) );
    XMVECTOR y3 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._31) );
    XMVECTOR y4 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._41) );

    x1 = XMVectorSubtract( x1, y1 );
    x2 = XMVectorSubtract( x2, y2 );
    x3 = XMVectorSubtract( x3, y3 );
    x4 = XMVectorSubtract( x4, y4 );

    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&_11), x1 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&_21), x2 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&_31), x3 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&_41), x4 );
    return *this;
}

inline Float4x4& Float4x4::operator*= (const Float4x4& M)
{
    using namespace DirectX;
    XMMATRIX M1 = XMLoadFloat4x4( this );
    XMMATRIX M2 = XMLoadFloat4x4( &M );
    XMMATRIX X = XMMatrixMultiply( M1, M2 );
    XMStoreFloat4x4( this, X );
    return *this;
}

inline Float4x4& Float4x4::operator*= (float S)
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( reinterpret_cast<XMFLOAT4*>(&_11) );
    XMVECTOR x2 = XMLoadFloat4( reinterpret_cast<XMFLOAT4*>(&_21) );
    XMVECTOR x3 = XMLoadFloat4( reinterpret_cast<XMFLOAT4*>(&_31) );
    XMVECTOR x4 = XMLoadFloat4( reinterpret_cast<XMFLOAT4*>(&_41) );

    x1 = XMVectorScale( x1, S );
    x2 = XMVectorScale( x2, S );
    x3 = XMVectorScale( x3, S );
    x4 = XMVectorScale( x4, S );

    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&_11), x1 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&_21), x2 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&_31), x3 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&_41), x4 );
    return *this;
}

inline Float4x4& Float4x4::operator/= (float S)
{
    using namespace DirectX;
    assert( S != 0.f );
    XMVECTOR x1 = XMLoadFloat4( reinterpret_cast<XMFLOAT4*>(&_11) );
    XMVECTOR x2 = XMLoadFloat4( reinterpret_cast<XMFLOAT4*>(&_21) );
    XMVECTOR x3 = XMLoadFloat4( reinterpret_cast<XMFLOAT4*>(&_31) );
    XMVECTOR x4 = XMLoadFloat4( reinterpret_cast<XMFLOAT4*>(&_41) );

    float rs = 1.f / S;

    x1 = XMVectorScale( x1, rs );
    x2 = XMVectorScale( x2, rs );
    x3 = XMVectorScale( x3, rs );
    x4 = XMVectorScale( x4, rs );

    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&_11), x1 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&_21), x2 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&_31), x3 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&_41), x4 );
    return *this;
}

inline Float4x4& Float4x4::operator/= (const Float4x4& M)
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( reinterpret_cast<XMFLOAT4*>(&_11) );
    XMVECTOR x2 = XMLoadFloat4( reinterpret_cast<XMFLOAT4*>(&_21) );
    XMVECTOR x3 = XMLoadFloat4( reinterpret_cast<XMFLOAT4*>(&_31) );
    XMVECTOR x4 = XMLoadFloat4( reinterpret_cast<XMFLOAT4*>(&_41) );

    XMVECTOR y1 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._11) );
    XMVECTOR y2 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._21) );
    XMVECTOR y3 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._31) );
    XMVECTOR y4 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._41) );

    x1 = XMVectorDivide( x1, y1 );
    x2 = XMVectorDivide( x2, y2 );
    x3 = XMVectorDivide( x3, y3 );
    x4 = XMVectorDivide( x4, y4 );

    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&_11), x1 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&_21), x2 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&_31), x3 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&_41), x4 );
    return *this;
}

//------------------------------------------------------------------------------
// Urnary operators
//------------------------------------------------------------------------------

inline Float4x4 Float4x4::operator- () const
{
    using namespace DirectX;
    XMVECTOR v1 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&_11) );
    XMVECTOR v2 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&_21) );
    XMVECTOR v3 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&_31) );
    XMVECTOR v4 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&_41) );

    v1 = XMVectorNegate( v1 );
    v2 = XMVectorNegate( v2 );
    v3 = XMVectorNegate( v3 );
    v4 = XMVectorNegate( v4 );

    Float4x4 R;
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._11), v1 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._21), v2 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._31), v3 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._41), v4 );
    return R;
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

inline Float4x4 operator+ (const Float4x4& M1, const Float4x4& M2)
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M1._11) );
    XMVECTOR x2 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M1._21) );
    XMVECTOR x3 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M1._31) );
    XMVECTOR x4 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M1._41) );

    XMVECTOR y1 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M2._11) );
    XMVECTOR y2 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M2._21) );
    XMVECTOR y3 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M2._31) );
    XMVECTOR y4 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M2._41) );

    x1 = XMVectorAdd( x1, y1 );
    x2 = XMVectorAdd( x2, y2 );
    x3 = XMVectorAdd( x3, y3 );
    x4 = XMVectorAdd( x4, y4 );

    Float4x4 R;
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._11), x1 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._21), x2 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._31), x3 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._41), x4 );
    return R;
}

inline Float4x4 operator- (const Float4x4& M1, const Float4x4& M2)
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M1._11) );
    XMVECTOR x2 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M1._21) );
    XMVECTOR x3 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M1._31) );
    XMVECTOR x4 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M1._41) );

    XMVECTOR y1 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M2._11) );
    XMVECTOR y2 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M2._21) );
    XMVECTOR y3 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M2._31) );
    XMVECTOR y4 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M2._41) );

    x1 = XMVectorSubtract( x1, y1 );
    x2 = XMVectorSubtract( x2, y2 );
    x3 = XMVectorSubtract( x3, y3 );
    x4 = XMVectorSubtract( x4, y4 );

    Float4x4 R;
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._11), x1 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._21), x2 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._31), x3 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._41), x4 );
    return R;
}

inline Float4x4 operator* (const Float4x4& M1, const Float4x4& M2)
{
    using namespace DirectX;
    XMMATRIX m1 = XMLoadFloat4x4( &M1 );
    XMMATRIX m2 = XMLoadFloat4x4( &M2 );
    XMMATRIX X = XMMatrixMultiply( m1, m2 );

    Float4x4 R;
    XMStoreFloat4x4( &R, X );
    return R;
}

inline Float4x4 operator* (const Float4x4& M, float S)
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._11) );
    XMVECTOR x2 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._21) );
    XMVECTOR x3 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._31) );
    XMVECTOR x4 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._41) );

    x1 = XMVectorScale( x1, S );
    x2 = XMVectorScale( x2, S );
    x3 = XMVectorScale( x3, S );
    x4 = XMVectorScale( x4, S );

    Float4x4 R;
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._11), x1 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._21), x2 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._31), x3 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._41), x4 );
    return R;
}

inline Float4x4 operator/ (const Float4x4& M, float S)
{
    using namespace DirectX;
    assert( S != 0.f );

    XMVECTOR x1 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._11) );
    XMVECTOR x2 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._21) );
    XMVECTOR x3 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._31) );
    XMVECTOR x4 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._41) );

    float rs = 1.f / S;

    x1 = XMVectorScale( x1, rs );
    x2 = XMVectorScale( x2, rs );
    x3 = XMVectorScale( x3, rs );
    x4 = XMVectorScale( x4, rs );

    Float4x4 R;
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._11), x1 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._21), x2 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._31), x3 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._41), x4 );
    return R;
}

inline Float4x4 operator/ (const Float4x4& M1, const Float4x4& M2)
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M1._11) );
    XMVECTOR x2 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M1._21) );
    XMVECTOR x3 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M1._31) );
    XMVECTOR x4 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M1._41) );

    XMVECTOR y1 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M2._11) );
    XMVECTOR y2 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M2._21) );
    XMVECTOR y3 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M2._31) );
    XMVECTOR y4 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M2._41) );

    x1 = XMVectorDivide( x1, y1 );
    x2 = XMVectorDivide( x2, y2 );
    x3 = XMVectorDivide( x3, y3 );
    x4 = XMVectorDivide( x4, y4 );

    Float4x4 R;
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._11), x1 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._21), x2 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._31), x3 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._41), x4 );
    return R;
}

inline Float4x4 operator* (float S, const Float4x4& M)
{
    using namespace DirectX;

    XMVECTOR x1 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._11) );
    XMVECTOR x2 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._21) );
    XMVECTOR x3 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._31) );
    XMVECTOR x4 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M._41) );

    x1 = XMVectorScale( x1, S );
    x2 = XMVectorScale( x2, S );
    x3 = XMVectorScale( x3, S );
    x4 = XMVectorScale( x4, S );

    Float4x4 R;
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._11), x1 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._21), x2 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._31), x3 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&R._41), x4 );
    return R;
}

//------------------------------------------------------------------------------
// Matrix operations
//------------------------------------------------------------------------------

inline bool Float4x4::Decompose( Float3& scale, Quaternion& rotation, Float3& translation )
{
    using namespace DirectX;

    XMVECTOR s, r, t;

    if ( !XMMatrixDecompose( &s, &r, &t, *this ) )
        return false;

    XMStoreFloat3( &scale, s );
    XMStoreFloat4( &rotation, r );
    XMStoreFloat3( &translation, t );

    return true;
}

inline Float4x4 Float4x4::Transpose() const
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4( this );
    Float4x4 R;
    XMStoreFloat4x4( &R, XMMatrixTranspose( M ) );
    return R;
}

inline void Float4x4::Transpose( Float4x4& result ) const
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4( this );
    XMStoreFloat4x4( &result, XMMatrixTranspose( M ) );
}

inline Float4x4 Float4x4::Invert() const
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4( this );
    Float4x4 R;
    XMVECTOR det;
    XMStoreFloat4x4( &R, XMMatrixInverse( &det, M ) );
    return R;
}

inline void Float4x4::Invert( Float4x4& result ) const
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4( this );
    XMVECTOR det;
    XMStoreFloat4x4( &result, XMMatrixInverse( &det, M ) );
}

inline float Float4x4::Determinant() const
{
    using namespace DirectX;
    XMMATRIX M = XMLoadFloat4x4( this );
    return XMVectorGetX( XMMatrixDeterminant( M ) );
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

inline Float4x4 Float4x4::Identity()
{
    using namespace DirectX;
    return Float4x4( 1.f,   0,   0,   0,
                   0,   1.f,   0,   0,
                   0,     0, 1.f,   0,
                   0,     0,   0, 1.f );
}

inline Float4x4 Float4x4::CreateTranslation( const Float3& position )
{
    using namespace DirectX;
    Float4x4 R;
    XMStoreFloat4x4( &R, XMMatrixTranslation( position.x, position.y, position.z ) );
    return R;
}

inline Float4x4 Float4x4::CreateTranslation( float x, float y, float z )
{
    using namespace DirectX;
    Float4x4 R;
    XMStoreFloat4x4( &R, XMMatrixTranslation( x, y, z ) );
    return R;
}

inline Float4x4 Float4x4::CreateScale( const Float3& scales )
{
    using namespace DirectX;
    Float4x4 R;
    XMStoreFloat4x4( &R, XMMatrixScaling( scales.x, scales.y, scales.z ) );
    return R;
}

inline Float4x4 Float4x4::CreateScale( float xs, float ys, float zs )
{
    using namespace DirectX;
    Float4x4 R;
    XMStoreFloat4x4( &R, XMMatrixScaling( xs, ys, zs ) );
    return R;
}

inline Float4x4 Float4x4::CreateScale( float scale )
{
    using namespace DirectX;
    Float4x4 R;
    XMStoreFloat4x4( &R, XMMatrixScaling( scale, scale, scale ) );
    return R;
}

inline Float4x4 Float4x4::CreateRotationX( float radians )
{
    using namespace DirectX;
    Float4x4 R;
    XMStoreFloat4x4( &R, XMMatrixRotationX( radians ) );
    return R;
}

inline Float4x4 Float4x4::CreateRotationY( float radians )
{
    using namespace DirectX;
    Float4x4 R;
    XMStoreFloat4x4( &R, XMMatrixRotationY( radians ) );
    return R;
}

inline Float4x4 Float4x4::CreateRotationZ( float radians )
{
    using namespace DirectX;
    Float4x4 R;
    XMStoreFloat4x4( &R, XMMatrixRotationZ( radians ) );
    return R;
}

inline Float4x4 Float4x4::CreateFromAxisAngle( const Float3& axis, float angle )
{
    using namespace DirectX;
    Float4x4 R;
    XMVECTOR a = XMLoadFloat3( &axis );
    XMStoreFloat4x4( &R, XMMatrixRotationAxis( a, angle ) );
    return R;
}

inline Float4x4 Float4x4::CreatePerspectiveFieldOfView( float fov, float aspectRatio, float nearPlane, float farPlane )
{
    using namespace DirectX;
    Float4x4 R;
    XMStoreFloat4x4( &R, XMMatrixPerspectiveFovRH( fov, aspectRatio, nearPlane, farPlane ) );
    return R;
}

inline Float4x4 Float4x4::CreatePerspective( float width, float height, float nearPlane, float farPlane )
{
    using namespace DirectX;
    Float4x4 R;
    XMStoreFloat4x4( &R, XMMatrixPerspectiveRH( width, height, nearPlane, farPlane ) );
    return R;
}

inline Float4x4 Float4x4::CreatePerspectiveOffCenter( float left, float right, float bottom, float top, float nearPlane, float farPlane )
{
    using namespace DirectX;
    Float4x4 R;
    XMStoreFloat4x4( &R, XMMatrixPerspectiveOffCenterRH( left, right, bottom, top, nearPlane, farPlane ) );
    return R;
}

inline Float4x4 Float4x4::CreateOrthographic( float width, float height, float zNearPlane, float zFarPlane )
{
    using namespace DirectX;
    Float4x4 R;
    XMStoreFloat4x4( &R, XMMatrixOrthographicRH( width, height, zNearPlane, zFarPlane ) );
    return R;
}

inline Float4x4 Float4x4::CreateOrthographicOffCenter( float left, float right, float bottom, float top, float zNearPlane, float zFarPlane )
{
    using namespace DirectX;
    Float4x4 R;
    XMStoreFloat4x4( &R, XMMatrixOrthographicOffCenterRH( left, right, bottom, top, zNearPlane, zFarPlane ) );
    return R;
}

inline Float4x4 Float4x4::CreateLookAt( const Float3& eye, const Float3& target, const Float3& up )
{
    using namespace DirectX;
    Float4x4 R;
    XMVECTOR eyev = XMLoadFloat3( &eye );
    XMVECTOR targetv = XMLoadFloat3( &target );
    XMVECTOR upv = XMLoadFloat3( &up );
    XMStoreFloat4x4( &R, XMMatrixLookAtRH( eyev, targetv, upv ) );
    return R;
}

inline Float4x4 Float4x4::CreateWorld( const Float3& position, const Float3& forward, const Float3& up )
{
    using namespace DirectX;
    XMVECTOR zaxis = XMVector3Normalize( XMVectorNegate( XMLoadFloat3( &forward ) ) );
    XMVECTOR yaxis = XMLoadFloat3( &up );
    XMVECTOR xaxis = XMVector3Normalize( XMVector3Cross( yaxis, zaxis ) );
    yaxis = XMVector3Cross( zaxis, xaxis );
    
    Float4x4 R;
    XMStoreFloat3( reinterpret_cast<XMFLOAT3*>( &R._11 ), xaxis );
    XMStoreFloat3( reinterpret_cast<XMFLOAT3*>( &R._21 ), yaxis );
    XMStoreFloat3( reinterpret_cast<XMFLOAT3*>( &R._31 ), zaxis );
    R._14 = R._24 = R._34 = 0.f;
    R._41 = position.x; R._42 = position.y; R._43 = position.z;
    R._44 = 1.f;
    return R;
}

inline Float4x4 Float4x4::CreateFromQuaternion( const Quaternion& rotation )
{
    using namespace DirectX;
    Float4x4 R;
    XMVECTOR quatv = XMLoadFloat4( &rotation );
    XMStoreFloat4x4( &R, XMMatrixRotationQuaternion( quatv ) );
    return R;
}

inline Float4x4 Float4x4::CreateFromYawPitchRoll( float yaw, float pitch, float roll )
{
    using namespace DirectX;
    Float4x4 R;
    XMStoreFloat4x4( &R, XMMatrixRotationRollPitchYaw( pitch, yaw, roll ) );
    return R;
}

inline Float4x4 Float4x4::CreateShadow( const Float3& lightDir, const Plane& plane )
{
    using namespace DirectX;
    Float4x4 R;
    XMVECTOR light = XMLoadFloat3( &lightDir );
    XMVECTOR planev = XMLoadFloat4( &plane );
    XMStoreFloat4x4( &R, XMMatrixShadow( planev, light ) );
    return R;
}

inline Float4x4 Float4x4::CreateReflection( const Plane& plane )
{
    using namespace DirectX;
    Float4x4 R;
    XMVECTOR planev = XMLoadFloat4( &plane );
    XMStoreFloat4x4( &R, XMMatrixReflect( planev ) );
    return R;
}

inline void Float4x4::Lerp( const Float4x4& M1, const Float4x4& M2, float t, Float4x4& result )
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M1._11) );
    XMVECTOR x2 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M1._21) );
    XMVECTOR x3 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M1._31) );
    XMVECTOR x4 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M1._41) );

    XMVECTOR y1 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M2._11) );
    XMVECTOR y2 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M2._21) );
    XMVECTOR y3 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M2._31) );
    XMVECTOR y4 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M2._41) );

    x1 = XMVectorLerp( x1, y1, t );
    x2 = XMVectorLerp( x2, y2, t );
    x3 = XMVectorLerp( x3, y3, t );
    x4 = XMVectorLerp( x4, y4, t );

    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&result._11), x1 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&result._21), x2 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&result._31), x3 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&result._41), x4 );
}

inline Float4x4 Float4x4::Lerp( const Float4x4& M1, const Float4x4& M2, float t )
{
    using namespace DirectX;
    XMVECTOR x1 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M1._11) );
    XMVECTOR x2 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M1._21) );
    XMVECTOR x3 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M1._31) );
    XMVECTOR x4 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M1._41) );

    XMVECTOR y1 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M2._11) );
    XMVECTOR y2 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M2._21) );
    XMVECTOR y3 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M2._31) );
    XMVECTOR y4 = XMLoadFloat4( reinterpret_cast<const XMFLOAT4*>(&M2._41) );

    x1 = XMVectorLerp( x1, y1, t );
    x2 = XMVectorLerp( x2, y2, t );
    x3 = XMVectorLerp( x3, y3, t );
    x4 = XMVectorLerp( x4, y4, t );

    Float4x4 result;
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&result._11), x1 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&result._21), x2 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&result._31), x3 );
    XMStoreFloat4( reinterpret_cast<XMFLOAT4*>(&result._41), x4 );
    return result;
}

inline void Float4x4::Transform( const Float4x4& M, const Quaternion& rotation, Float4x4& result )
{
    using namespace DirectX;
    XMVECTOR quatv = XMLoadFloat4( &rotation );

    XMMATRIX M0 = XMLoadFloat4x4( &M );
    XMMATRIX M1 = XMMatrixRotationQuaternion( quatv );

    XMStoreFloat4x4( &result, XMMatrixMultiply( M0, M1 ) );
}

inline Float4x4 Float4x4::Transform( const Float4x4& M, const Quaternion& rotation )
{
    using namespace DirectX;
    XMVECTOR quatv = XMLoadFloat4( &rotation );

    XMMATRIX M0 = XMLoadFloat4x4( &M );
    XMMATRIX M1 = XMMatrixRotationQuaternion( quatv );

    Float4x4 result;
    XMStoreFloat4x4( &result, XMMatrixMultiply( M0, M1 ) );
    return result;
}

#pragma endregion

#pragma region Plane
/****************************************************************************
 *
 * Plane
 *
 ****************************************************************************/

inline Plane::Plane(const Float3& point1, const Float3& point2, const Float3& point3 )
{
    using namespace DirectX;
    XMVECTOR P0 = XMLoadFloat3( &point1 );
    XMVECTOR P1 = XMLoadFloat3( &point2 );
    XMVECTOR P2 = XMLoadFloat3( &point3 );
    XMStoreFloat4( this, XMPlaneFromPoints( P0, P1, P2 ) );
}

inline Plane::Plane(const Float3& point, const Float3& normal)
{
    using namespace DirectX;
    XMVECTOR P = XMLoadFloat3( &point );
    XMVECTOR N = XMLoadFloat3( &normal );
    XMStoreFloat4( this, XMPlaneFromPointNormal( P, N ) );
}

inline bool Plane::operator == ( const Plane& p ) const
{
    using namespace DirectX;
    XMVECTOR p1 = XMLoadFloat4( this );
    XMVECTOR p2 = XMLoadFloat4( &p );
    return XMPlaneEqual( p1, p2 );
}

inline bool Plane::operator != ( const Plane& p ) const
{
    using namespace DirectX;
    XMVECTOR p1 = XMLoadFloat4( this );
    XMVECTOR p2 = XMLoadFloat4( &p );
    return XMPlaneNotEqual( p1, p2 );
}

inline void Plane::Normalize()
{
    using namespace DirectX;
    XMVECTOR p = XMLoadFloat4( this );
    XMStoreFloat4( this, XMPlaneNormalize( p ) );
}

inline void Plane::Normalize( Plane& result ) const
{
    using namespace DirectX;
    XMVECTOR p = XMLoadFloat4( this );
    XMStoreFloat4( &result, XMPlaneNormalize( p ) );
}

inline float Plane::Dot( const Float4& v ) const
{
    using namespace DirectX;
    XMVECTOR p = XMLoadFloat4( this );
    XMVECTOR v0 = XMLoadFloat4( &v );
    return XMVectorGetX( XMPlaneDot( p, v0 ) );
}

inline float Plane::DotCoordinate( const Float3& position ) const
{
    using namespace DirectX;
    XMVECTOR p = XMLoadFloat4( this );
    XMVECTOR v0 = XMLoadFloat3( &position );
    return XMVectorGetX( XMPlaneDotCoord( p, v0 ) );
}

inline float Plane::DotNormal( const Float3& normal ) const
{
    using namespace DirectX;
    XMVECTOR p = XMLoadFloat4( this );
    XMVECTOR n0 = XMLoadFloat3( &normal );
    return XMVectorGetX( XMPlaneDotNormal( p, n0 ) );
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

inline void Plane::Transform( const Plane& plane, const Float4x4& M, Plane& result )
{
    using namespace DirectX;
    XMVECTOR p = XMLoadFloat4( &plane );
    XMMATRIX m0 = XMLoadFloat4x4( &M );
    XMStoreFloat4( &result, XMPlaneTransform( p, m0 ) );
}

inline Plane Plane::Transform( const Plane& plane, const Float4x4& M )
{
    using namespace DirectX;
    XMVECTOR p = XMLoadFloat4( &plane );
    XMMATRIX m0 = XMLoadFloat4x4( &M );

    Plane result;
    XMStoreFloat4( &result, XMPlaneTransform( p, m0 ) );
    return result;
}

inline void Plane::Transform( const Plane& plane, const Quaternion& rotation, Plane& result )
{
    using namespace DirectX;
    XMVECTOR p = XMLoadFloat4( &plane );
    XMVECTOR q = XMLoadFloat4( &rotation );
    XMVECTOR X = XMVector3Rotate( p, q );
    X = XMVectorSelect( p, X, g_XMSelect1110 ); // result.d = plane.d
    XMStoreFloat4( &result, X );
}

inline Plane Plane::Transform( const Plane& plane, const Quaternion& rotation )
{
    using namespace DirectX;
    XMVECTOR p = XMLoadFloat4( &plane );
    XMVECTOR q = XMLoadFloat4( &rotation );
    XMVECTOR X = XMVector3Rotate( p, q );
    X = XMVectorSelect( p, X, g_XMSelect1110 ); // result.d = plane.d

    Plane result;
    XMStoreFloat4( &result, X );
    return result;
}

#pragma endregion

#pragma region Quaternion
/****************************************************************************
 *
 * Quaternion
 *
 ****************************************************************************/
inline bool Quaternion::operator == ( const Quaternion& q ) const
{
   return (x==q.x && y==q.y && z == q.z && w==q.w);
}

inline bool Quaternion::operator != ( const Quaternion& q ) const
{
   return (x!=q.x || y!=q.y|| z!=q.z||w!=q.w);
}

inline Quaternion& Quaternion::operator+= (const Quaternion& q)
{
    using namespace DirectX;
    XMVECTOR q1 = XMLoadFloat4( this );
    XMVECTOR q2 = XMLoadFloat4( &q );
    XMStoreFloat4( this, XMVectorAdd( q1, q2 ) );
    return *this;
}

inline Quaternion& Quaternion::operator-= (const Quaternion& q)
{
    using namespace DirectX;
    XMVECTOR q1 = XMLoadFloat4( this );
    XMVECTOR q2 = XMLoadFloat4( &q );
    XMStoreFloat4( this, XMVectorSubtract( q1, q2 ) );
    return *this;
}

inline Quaternion& Quaternion::operator*= (const Quaternion& q)
{
    using namespace DirectX;
    XMVECTOR q1 = XMLoadFloat4( this );
    XMVECTOR q2 = XMLoadFloat4( &q );
    XMStoreFloat4( this, XMQuaternionMultiply( q1, q2 ) );
    return *this;
}

inline Quaternion& Quaternion::operator*= (float S)
{
    using namespace DirectX;
    XMVECTOR q = XMLoadFloat4( this );
    XMStoreFloat4( this, XMVectorScale( q, S ) );
    return *this;
}

inline Quaternion& Quaternion::operator/= (const Quaternion& q)
{
    using namespace DirectX;
    XMVECTOR q1 = XMLoadFloat4( this );
    XMVECTOR q2 = XMLoadFloat4( &q );
    q2 = XMQuaternionInverse( q2 );
    XMStoreFloat4( this, XMQuaternionMultiply( q1, q2 ) );
    return *this;
}

//------------------------------------------------------------------------------
// Urnary operators
//------------------------------------------------------------------------------

inline Quaternion Quaternion::operator- () const
{
    using namespace DirectX;
    XMVECTOR q = XMLoadFloat4( this );

    Quaternion R;
    XMStoreFloat4( &R, XMVectorNegate( q ) );
    return R;
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

inline Quaternion operator+ (const Quaternion& Q1, const Quaternion& Q2)
{
    using namespace DirectX;
    XMVECTOR q1 = XMLoadFloat4( &Q1 );
    XMVECTOR q2 = XMLoadFloat4( &Q2 );

    Quaternion R;
    XMStoreFloat4( &R, XMVectorAdd( q1, q2 ) );
    return R;
}

inline Quaternion operator- (const Quaternion& Q1, const Quaternion& Q2)
{
    using namespace DirectX;
    XMVECTOR q1 = XMLoadFloat4( &Q1 );
    XMVECTOR q2 = XMLoadFloat4( &Q2 );

    Quaternion R;
    XMStoreFloat4( &R, XMVectorSubtract( q1, q2 ) );
    return R;
}

inline Quaternion operator* (const Quaternion& Q1, const Quaternion& Q2)
{
    using namespace DirectX;
    XMVECTOR q1 = XMLoadFloat4( &Q1 );
    XMVECTOR q2 = XMLoadFloat4( &Q2 );

    Quaternion R;
    XMStoreFloat4( &R, XMQuaternionMultiply( q1, q2 ) );
    return R;
}

inline Quaternion operator* (const Quaternion& Q, float S)
{
    using namespace DirectX;
    XMVECTOR q = XMLoadFloat4( &Q );

    Quaternion R;
    XMStoreFloat4( &R, XMVectorScale( q, S ) );
    return R;
}

inline Quaternion operator/ (const Quaternion& Q1, const Quaternion& Q2)
{
    using namespace DirectX;
    XMVECTOR q1 = XMLoadFloat4( &Q1 );
    XMVECTOR q2 = XMLoadFloat4( &Q2 );
    q2 = XMQuaternionInverse( q2 );

    Quaternion R;
    XMStoreFloat4( &R, XMQuaternionMultiply( q1, q2 ) );
    return R;
}

inline Quaternion operator* (float S, const Quaternion& Q)
{
    using namespace DirectX;
    XMVECTOR q1 = XMLoadFloat4( &Q );

    Quaternion R;
    XMStoreFloat4( &R, XMVectorScale( q1, S ) );
    return R;
}

//------------------------------------------------------------------------------
// Quaternion operations
//------------------------------------------------------------------------------

inline float Quaternion::Length() const
{
    using namespace DirectX;
    XMVECTOR q = XMLoadFloat4( this );
    return XMVectorGetX( XMQuaternionLength( q ) );
}

inline float Quaternion::LengthSquared() const
{
    using namespace DirectX;
    XMVECTOR q = XMLoadFloat4( this );
    return XMVectorGetX( XMQuaternionLengthSq( q ) );
}

inline void Quaternion::Normalize()
{
    using namespace DirectX;
    XMVECTOR q = XMLoadFloat4( this );
    XMStoreFloat4( this, XMQuaternionNormalize( q ) );
}

inline void Quaternion::Normalize( Quaternion& result ) const
{
    using namespace DirectX;
    XMVECTOR q = XMLoadFloat4( this );
    XMStoreFloat4( &result, XMQuaternionNormalize( q ) );
}

inline void Quaternion::Conjugate()
{
    using namespace DirectX;
    XMVECTOR q = XMLoadFloat4( this );
    XMStoreFloat4( this, XMQuaternionConjugate( q ) );
}

inline void Quaternion::Conjugate( Quaternion& result ) const
{
    using namespace DirectX;
    XMVECTOR q = XMLoadFloat4( this );
    XMStoreFloat4( &result, XMQuaternionConjugate( q ) );
}

inline void Quaternion::Inverse( Quaternion& result ) const
{
    using namespace DirectX;
    XMVECTOR q = XMLoadFloat4( this );
    XMStoreFloat4( &result, XMQuaternionInverse( q ) );
}

inline float Quaternion::Dot( const Quaternion& q ) const
{
    using namespace DirectX;
    XMVECTOR q1 = XMLoadFloat4( this );
    XMVECTOR q2 = XMLoadFloat4( &q );
    return XMVectorGetX( XMQuaternionDot( q1, q2 ) );
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

inline Quaternion Quaternion::CreateFromAxisAngle( const Float3& axis, float angle )
{
    using namespace DirectX;
    XMVECTOR a = XMLoadFloat3( &axis );

    Quaternion R;
    XMStoreFloat4( &R, XMQuaternionRotationAxis( a, angle ) );
    return R;
}

inline Quaternion Quaternion::CreateFromYawPitchRoll( float yaw, float pitch, float roll )
{
    using namespace DirectX;
    Quaternion R;
    XMStoreFloat4( &R, XMQuaternionRotationRollPitchYaw( pitch, yaw, roll ) );
    return R;
}

inline Quaternion Quaternion::CreateFromRotationMatrix( const Float4x4& M )
{
    using namespace DirectX;
    XMMATRIX M0 = XMLoadFloat4x4( &M );

    Quaternion R;
    XMStoreFloat4( &R, XMQuaternionRotationMatrix( M0 ) );
    return R;
}

inline void Quaternion::Lerp( const Quaternion& q1, const Quaternion& q2, float t, Quaternion& result )
{
    using namespace DirectX;
    XMVECTOR Q0 = XMLoadFloat4( &q1 );
    XMVECTOR Q1 = XMLoadFloat4( &q2 );

    XMVECTOR dot = XMVector4Dot( Q0, Q1 );

    XMVECTOR R;
    if ( XMVector4GreaterOrEqual( dot, XMVectorZero() ) )
    {
        R = XMVectorLerp( Q0, Q1, t );
    }
    else
    {
        XMVECTOR tv = XMVectorReplicate( t );
        XMVECTOR t1v = XMVectorReplicate( 1.f - t );
        XMVECTOR X0 = XMVectorMultiply( Q0, t1v );
        XMVECTOR X1 = XMVectorMultiply( Q1, tv );
        R = XMVectorSubtract( X0, X1 );
    }

    XMStoreFloat4( &result, XMQuaternionNormalize( R ) );
}

inline Quaternion Quaternion::Lerp( const Quaternion& q1, const Quaternion& q2, float t )
{
    using namespace DirectX;
    XMVECTOR Q0 = XMLoadFloat4( &q1 );
    XMVECTOR Q1 = XMLoadFloat4( &q2 );

    XMVECTOR dot = XMVector4Dot( Q0, Q1 );

    XMVECTOR R;
    if ( XMVector4GreaterOrEqual( dot, XMVectorZero() ) )
    {
        R = XMVectorLerp( Q0, Q1, t );
    }
    else
    {
        XMVECTOR tv = XMVectorReplicate( t );
        XMVECTOR t1v = XMVectorReplicate( 1.f - t );
        XMVECTOR X0 = XMVectorMultiply( Q0, t1v );
        XMVECTOR X1 = XMVectorMultiply( Q1, tv );
        R = XMVectorSubtract( X0, X1 );
    }

    Quaternion result;
    XMStoreFloat4( &result, XMQuaternionNormalize( R ) );
    return result;
}

inline void Quaternion::Slerp( const Quaternion& q1, const Quaternion& q2, float t, Quaternion& result )
{
    using namespace DirectX;
    XMVECTOR Q0 = XMLoadFloat4( &q1 );
    XMVECTOR Q1 = XMLoadFloat4( &q2 );
    XMStoreFloat4( &result, XMQuaternionSlerp( Q0, Q1, t ) );
}

inline Quaternion Quaternion::Slerp( const Quaternion& q1, const Quaternion& q2, float t )
{
    using namespace DirectX;
    XMVECTOR Q0 = XMLoadFloat4( &q1 );
    XMVECTOR Q1 = XMLoadFloat4( &q2 );

    Quaternion result;
    XMStoreFloat4( &result, XMQuaternionSlerp( Q0, Q1, t ) );
    return result;
}

inline void Quaternion::Concatenate( const Quaternion& q1, const Quaternion& q2, Quaternion& result )
{
    using namespace DirectX;
    XMVECTOR Q0 = XMLoadFloat4( &q1 );
    XMVECTOR Q1 = XMLoadFloat4( &q2 );
    XMStoreFloat4( &result, XMQuaternionMultiply( Q1, Q0 ) );
}

inline Quaternion Quaternion::Concatenate( const Quaternion& q1, const Quaternion& q2 )
{
    using namespace DirectX;
    XMVECTOR Q0 = XMLoadFloat4( &q1 );
    XMVECTOR Q1 = XMLoadFloat4( &q2 );

    Quaternion result;
    XMStoreFloat4( &result, XMQuaternionMultiply( Q1, Q0 ) );
    return result;
}
#pragma endregion

#pragma region Color32
/****************************************************************************
 *
 * Color32
 *
 ****************************************************************************/

inline Color32::Color32( const DirectX::PackedVector::XMCOLOR& Packed )
{
    using namespace DirectX;
    XMStoreFloat4( this, PackedVector::XMLoadColor( &Packed ) );
}

inline Color32::Color32( const DirectX::PackedVector::XMUBYTEN4& Packed )
{
    using namespace DirectX;
    XMStoreFloat4( this, PackedVector::XMLoadUByteN4( &Packed ) );
}

inline bool Color32::operator == ( const Color32& c ) const
{
    using namespace DirectX;
    XMVECTOR c1 = XMLoadFloat4( this );
    XMVECTOR c2 = XMLoadFloat4( &c );
    return XMColorEqual( c1, c2 );
}

inline bool Color32::operator != ( const Color32& c ) const
{
    using namespace DirectX;
    XMVECTOR c1 = XMLoadFloat4( this );
    XMVECTOR c2 = XMLoadFloat4( &c );
    return XMColorNotEqual( c1, c2 );
}

inline Color32& Color32::operator+= (const Color32& c)
{
    using namespace DirectX;
    XMVECTOR c1 = XMLoadFloat4( this );
    XMVECTOR c2 = XMLoadFloat4( &c );
    XMStoreFloat4( this, XMVectorAdd( c1, c2 ) );
    return *this;
}

inline Color32& Color32::operator-= (const Color32& c)
{
    using namespace DirectX;
    XMVECTOR c1 = XMLoadFloat4( this );
    XMVECTOR c2 = XMLoadFloat4( &c );
    XMStoreFloat4( this, XMVectorSubtract( c1, c2 ) );
    return *this;
}

inline Color32& Color32::operator*= (const Color32& c)
{
    using namespace DirectX;
    XMVECTOR c1 = XMLoadFloat4( this );
    XMVECTOR c2 = XMLoadFloat4( &c );
    XMStoreFloat4( this, XMVectorMultiply( c1, c2 ) );
    return *this;
}

inline Color32& Color32::operator*= (float S)
{
    using namespace DirectX;
    XMVECTOR c = XMLoadFloat4( this );
    XMStoreFloat4( this, XMVectorScale( c, S ) );
    return *this;
}

inline Color32& Color32::operator/= (const Color32& c)
{
    using namespace DirectX;
    XMVECTOR c1 = XMLoadFloat4( this );
    XMVECTOR c2 = XMLoadFloat4( &c );
    XMStoreFloat4( this, XMVectorDivide( c1, c2 ) );
    return *this;
}

//------------------------------------------------------------------------------
// Urnary operators
//------------------------------------------------------------------------------

inline Color32 Color32::operator- () const
{
    using namespace DirectX;
    XMVECTOR c = XMLoadFloat4( this );
    Color32 R;
    XMStoreFloat4( &R, XMVectorNegate( c ) );
    return R;
}

//------------------------------------------------------------------------------
// Binary operators
//------------------------------------------------------------------------------

inline Color32 operator+ (const Color32& C1, const Color32& C2)
{
    using namespace DirectX;
    XMVECTOR c1 = XMLoadFloat4( &C1 );
    XMVECTOR c2 = XMLoadFloat4( &C2 );
    Color32 R;
    XMStoreFloat4( &R, XMVectorAdd( c1, c2 ) );
    return R;
}

inline Color32 operator- (const Color32& C1, const Color32& C2)
{
    using namespace DirectX;
    XMVECTOR c1 = XMLoadFloat4( &C1 );
    XMVECTOR c2 = XMLoadFloat4( &C2 );
    Color32 R;
    XMStoreFloat4( &R, XMVectorSubtract( c1, c2 ) );
    return R;
}

inline Color32 operator* (const Color32& C1, const Color32& C2)
{
    using namespace DirectX;
    XMVECTOR c1 = XMLoadFloat4( &C1 );
    XMVECTOR c2 = XMLoadFloat4( &C2 );
    Color32 R;
    XMStoreFloat4( &R, XMVectorMultiply( c1, c2 ) );
    return R;
}

inline Color32 operator* (const Color32& C, float S)
{
    using namespace DirectX;
    XMVECTOR c = XMLoadFloat4( &C );
    Color32 R;
    XMStoreFloat4( &R, XMVectorScale( c, S ) );
    return R;
}

inline Color32 operator/ (const Color32& C1, const Color32& C2)
{
    using namespace DirectX;
    XMVECTOR c1 = XMLoadFloat4( &C1 );
    XMVECTOR c2 = XMLoadFloat4( &C2 );
    Color32 R;
    XMStoreFloat4( &R, XMVectorDivide( c1, c2 ) );
    return R;
}

inline Color32 operator* (float S, const Color32& C)
{
    using namespace DirectX;
    XMVECTOR c1 = XMLoadFloat4( &C );
    Color32 R;
    XMStoreFloat4( &R, XMVectorScale( c1, S ) );
    return R;
}

inline Color8 Color32::BGRA() const
{
    using namespace DirectX;
    XMVECTOR clr = XMLoadFloat4( this );
    PackedVector::XMCOLOR Packed;
    PackedVector::XMStoreColor( &Packed, clr );
    return Packed;
}

inline Uint8_n4 Color32::RGBA() const
{
    using namespace DirectX;
    XMVECTOR clr = XMLoadFloat4( this );
    PackedVector::XMUBYTEN4 Packed;
    PackedVector::XMStoreUByteN4( &Packed, clr );
    return Packed;
}

inline Float3 Color32::ToFloat3() const
{
    return Float3( x, y, z );
}

inline Float4 Color32::ToFloat4() const
{
    return Float4( x, y, z, w );
}

inline void Color32::Negate()
{
    using namespace DirectX;
    XMVECTOR c = XMLoadFloat4( this );
    XMStoreFloat4( this, XMColorNegative( c) );
}

inline void Color32::Negate( Color32& result ) const
{
    using namespace DirectX;
    XMVECTOR c = XMLoadFloat4( this );
    XMStoreFloat4( &result, XMColorNegative( c ) );
}

inline void Color32::Saturate()
{
    using namespace DirectX;
    XMVECTOR c = XMLoadFloat4( this );
    XMStoreFloat4( this, XMVectorSaturate( c ) );
}

inline void Color32::Saturate( Color32& result ) const
{
    using namespace DirectX;
    XMVECTOR c = XMLoadFloat4( this );
    XMStoreFloat4( &result, XMVectorSaturate( c ) );
}

inline void Color32::Premultiply()
{
    using namespace DirectX;
    XMVECTOR c = XMLoadFloat4( this );
    XMVECTOR a = XMVectorSplatW( c );
    a = XMVectorSelect( g_XMIdentityR3, a, g_XMSelect1110 );
    XMStoreFloat4( this, XMVectorMultiply( c, a ) );
}

inline void Color32::Premultiply( Color32& result ) const
{
    using namespace DirectX;
    XMVECTOR c = XMLoadFloat4( this );
    XMVECTOR a = XMVectorSplatW( c );
    a = XMVectorSelect( g_XMIdentityR3, a, g_XMSelect1110 );
    XMStoreFloat4( &result, XMVectorMultiply( c, a ) );
}

inline void Color32::AdjustSaturation( float sat )
{
    using namespace DirectX;
    XMVECTOR c = XMLoadFloat4( this );
    XMStoreFloat4( this, XMColorAdjustSaturation( c, sat ) );
}

inline void Color32::AdjustSaturation( float sat, Color32& result ) const
{
    using namespace DirectX;
    XMVECTOR c = XMLoadFloat4( this );
    XMStoreFloat4( &result, XMColorAdjustSaturation( c, sat ) );
}

inline void Color32::AdjustContrast( float contrast )
{
    using namespace DirectX;
    XMVECTOR c = XMLoadFloat4( this );
    XMStoreFloat4( this, XMColorAdjustContrast( c, contrast ) );
}

inline void Color32::AdjustContrast( float contrast, Color32& result ) const
{
    using namespace DirectX;
    XMVECTOR c = XMLoadFloat4( this );
    XMStoreFloat4( &result, XMColorAdjustContrast( c, contrast ) );
}

//------------------------------------------------------------------------------
// Static functions
//------------------------------------------------------------------------------

inline void Color32::Modulate( const Color32& c1, const Color32& c2, Color32& result )
{
    using namespace DirectX;
    XMVECTOR C0 = XMLoadFloat4( &c1 );
    XMVECTOR C1 = XMLoadFloat4( &c2 );
    XMStoreFloat4( &result, XMColorModulate( C0, C1 ) );
}

inline Color32 Color32::Modulate( const Color32& c1, const Color32& c2 )
{
    using namespace DirectX;
    XMVECTOR C0 = XMLoadFloat4( &c1 );
    XMVECTOR C1 = XMLoadFloat4( &c2 );

    Color32 result;
    XMStoreFloat4( &result, XMColorModulate( C0, C1 ) );
    return result;
}

inline void Color32::Lerp( const Color32& c1, const Color32& c2, float t, Color32& result )
{
    using namespace DirectX;
    XMVECTOR C0 = XMLoadFloat4( &c1 );
    XMVECTOR C1 = XMLoadFloat4( &c2 );
    XMStoreFloat4( &result, XMVectorLerp( C0, C1, t ) );
}

inline Color32 Color32::Lerp( const Color32& c1, const Color32& c2, float t )
{
    using namespace DirectX;
    XMVECTOR C0 = XMLoadFloat4( &c1 );
    XMVECTOR C1 = XMLoadFloat4( &c2 );

    Color32 result;
    XMStoreFloat4( &result, XMVectorLerp( C0, C1, t ) );
    return result;
}

#pragma endregion

#pragma region Ray
/****************************************************************************
 *
 * Ray
 *
 ****************************************************************************/

//-----------------------------------------------------------------------------
// Comparision operators
//------------------------------------------------------------------------------
inline bool Ray::operator == ( const Ray& r ) const
{
    using namespace DirectX;
    XMVECTOR r1p = XMLoadFloat3( &position );
    XMVECTOR r2p = XMLoadFloat3( &r.position );
    XMVECTOR r1d = XMLoadFloat3( &direction );
    XMVECTOR r2d = XMLoadFloat3( &r.direction );
    return XMVector3Equal( r1p, r2p ) && XMVector3Equal( r1d, r2d );
}

inline bool Ray::operator != ( const Ray& r ) const
{
    using namespace DirectX;
    XMVECTOR r1p = XMLoadFloat3( &position );
    XMVECTOR r2p = XMLoadFloat3( &r.position );
    XMVECTOR r1d = XMLoadFloat3( &direction );
    XMVECTOR r2d = XMLoadFloat3( &r.direction );
    return XMVector3NotEqual( r1p, r2p ) && XMVector3NotEqual( r1d, r2d );
}

//-----------------------------------------------------------------------------
// Ray operators
//------------------------------------------------------------------------------

inline bool Ray::Intersects( const BoundingSphere& sphere, _Out_ float& Dist ) const
{
    return sphere.Intersects( position, direction, Dist );
}

inline bool Ray::Intersects( const AABB& box, _Out_ float& Dist ) const
{
    return box.Intersects( position, direction, Dist );
}

inline bool Ray::Intersects( const Float3& tri0, const Float3& tri1, const Float3& tri2, _Out_ float& Dist ) const
{
    return DirectX::TriangleTests::Intersects( position, direction, tri0, tri1, tri2, Dist );
}

inline bool Ray::Intersects( const Plane& plane, _Out_ float& Dist ) const
{
    using namespace DirectX;

    XMVECTOR p = XMLoadFloat4( &plane );
    XMVECTOR dir = XMLoadFloat3( &direction );

    XMVECTOR nd = XMPlaneDotNormal( p, dir );

    if ( XMVector3LessOrEqual( XMVectorAbs( nd ), g_RayEpsilon ) )
    {
        Dist = 0.f;
        return false;
    }
    else
    {
        // t = -(dot(n,origin) + D) / dot(n,dir)
        XMVECTOR pos = XMLoadFloat3( &position );
        XMVECTOR v = XMPlaneDotNormal( p, pos );
        v = XMVectorAdd( v, XMVectorSplatW(p) );
        v = XMVectorDivide( v, nd );
        float dist = - XMVectorGetX( v );
        if (dist < 0)
        {
            Dist = 0.f;
            return false;
        }
        else
        {
            Dist = dist;
            return true;
        }
    }
}
#pragma endregion
