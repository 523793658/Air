
//////////////////////////////////////////////////////////////////////////
//Flame engine math library is based on DirectXMath ver 3.03 hotfix
//http://blogs.msdn.com/b/chuckw/archive/2013/03/06/known-issues-directxmath-3-03.aspx
//and SimpleMath http://directxtk.codeplex.com/ which is a simple wrapper around DXMath.
//We optimized SimpleMath, make it run fast for some simple vector operation.
//
//More detalis about DirectXMath can be found on
//http://blogs.msdn.com/b/chuckw/archive/2012/03/27/introducing-directxmath.aspx
//
//
//Care must be taken when using SIMD data types,*be sure* to read the DXMath document first:
//http://msdn.microsoft.com/en-us/library/windows/desktop/hh437833.aspx
//
//Clayman 2013.4.10
//////////////////////////////////////////////////////////////////////////


#pragma once

#pragma  warning(disable: 4838)

#include "DirectXMath.h"
#include "DirectXPackedVector.h"
#include "DirectXCollision.h"
#include <memory.h>

namespace Air
{
	
	typedef DirectX::XMVECTOR Vector4;
	typedef DirectX::XMMATRIX Matrix4x4;
		
	struct Float2;
	struct Float3;
	struct Float4;
	struct Float4x4;
	struct Quaternion;
	struct Color32;
	typedef DirectX::PackedVector::XMCOLOR Color8;
	typedef DirectX::PackedVector::XMUBYTEN4 Uint8_n4;

	typedef DirectX::XMFLOAT2A Float2A;
	typedef DirectX::XMFLOAT3A Float4A;
	typedef DirectX::XMFLOAT4A Float3A;

	typedef DirectX::XMUINT2 Uint2;
	typedef DirectX::XMUINT3 Uint3;
	typedef DirectX::XMUINT4 Uint4;

	typedef DirectX::XMINT2 Int2;
	typedef DirectX::XMINT3 Int3;
	typedef DirectX::XMINT4 Int4;

	typedef DirectX::XMFLOAT3X3  Float3x3;
	typedef DirectX::XMFLOAT4X3  Float4x3;
	typedef DirectX::XMFLOAT4X3A Float4x3A;
	typedef DirectX::XMFLOAT4X4A Float4x4A;

	typedef DirectX::BoundingBox AABB;
	typedef DirectX::BoundingOrientedBox OBB;
	typedef DirectX::BoundingSphere BoundingSphere;
	typedef DirectX::BoundingFrustum BoundingFrustum;
		
	/*********************************************************
	Remark:
	Use the Vector4Param alias to pass up to the first three instances of Vector4 used as arguments to a function.
	Use the Vector4Param4 alias to pass the 4th instance of an Vector4 used as an argument
	Use the Vector4ParamX alias to pass any further instances of Vector4 used as arguments.

	For output parameters, always use Vector4Param* or Vector4Param& and ignore them with respect 
	to the the rules above for input parameters.

	For 32-bit Windows:
	typedef const Vector4 FFloat4Simd;
	typedef const Vector4& Vector4Param4;
	typedef const Vector4& Vector4ParamX;

	For 64-bit Windows:
	typedef const Vector4& FFloat4Simd;
	typedef const Vector4& Vector4Param4;
	typedef const Vector4& Vector4ParamX;

	Windows on ARM:
	typedef const Vector4 FFloat4Simd;
	typedef const Vector4 Vector4Param4;
	typedef const Vector4& Vector4ParamX;
	see http://msdn.microsoft.com/en-us/library/ee418728(v=VS.85).aspx for more details
	*********************************************************/
	typedef DirectX::FXMVECTOR Vector4Param;
	typedef DirectX::GXMVECTOR Vector4Param4;
	typedef DirectX::CXMVECTOR Vector4ParamN;
	typedef DirectX::CXMMATRIX MatrixParam;

	
	//------------------------------------------------------------------------------
	// 2D vector
	struct Float2 :public DirectX::XMFLOAT2
	{
		Float2() : XMFLOAT2(0.f, 0.f) {}
		explicit Float2(float x) : XMFLOAT2( x, x ) {}
		Float2(float _x, float _y) : XMFLOAT2(_x, _y) {}
		explicit Float2( const float *pArray) : XMFLOAT2(pArray) {}

		operator Vector4() const { return XMLoadFloat2( this ); }

		// Comparision operators
		bool operator == ( const Float2& V ) const;
		bool operator != ( const Float2& V ) const;

		// Assignment operators
		Float2& operator= (const Float2& V) { x = V.x; y = V.y; return *this; }
		Float2& operator+= (const Float2& V);
		Float2& operator-= (const Float2& V);
		Float2& operator*= (const Float2& V);
		Float2& operator*= (float S);
		Float2& operator/= (float S);

		// Urnary operators
		Float2 operator+ () const { return *this; }
		Float2 operator- () const { return Float2(-x, -y); }

		// Vector operations
		bool InBounds( const Float2& Bounds ) const;

		float Length() const;
		float LengthSquared() const;

		float Dot( const Float2& V ) const;
		void Cross( const Float2& V, Float2& result ) const;
		Float2 Cross( const Float2& V ) const;

		void Normalize();
		void Normalize( Float2& result ) const;

		void Clamp( const Float2& vmin, const Float2& vmax );
		void Clamp( const Float2& vmin, const Float2& vmax, Float2& result ) const;

		// Static functions
		static float Distance( const Float2& v1, const Float2& v2 );
		static float DistanceSquared( const Float2& v1, const Float2& v2 );

		static void Min( const Float2& v1, const Float2& v2, Float2& result );
		static Float2 Min( const Float2& v1, const Float2& v2 );

		static void Max( const Float2& v1, const Float2& v2, Float2& result );
		static Float2 Max( const Float2& v1, const Float2& v2 );

		static void Lerp( const Float2& v1, const Float2& v2, float t, Float2& result );
		static Float2 Lerp( const Float2& v1, const Float2& v2, float t );

		//Interpolates between two values using a cubic equation
		static void SmoothStep( const Float2& v1, const Float2& v2, float t, Float2& result );
		static Float2 SmoothStep( const Float2& v1, const Float2& v2, float t );

		//Returns a Float2 containing the 2D Cartesian coordinates of a point specified in 2D barycentric (areal) coordinates.
		static void Barycentric( const Float2& v1, const Float2& v2, const Float2& v3, float f, float g, Float2& result );
		static Float2 Barycentric( const Float2& v1, const Float2& v2, const Float2& v3, float f, float g );

		//Performs a Catmull-Rom interpolation using the specified positions
		static void CatmullRom( const Float2& v1, const Float2& v2, const Float2& v3, const Float2& v4, float t, Float2& result );
		static Float2 CatmullRom( const Float2& v1, const Float2& v2, const Float2& v3, const Float2& v4, float t );

		static void Hermite( const Float2& v1, const Float2& t1, const Float2& v2, const Float2& t2, float t, Float2& result );
		static Float2 Hermite( const Float2& v1, const Float2& t1, const Float2& v2, const Float2& t2, float t );

		static void Reflect( const Float2& ivec, const Float2& nvec, Float2& result );
		static Float2 Reflect( const Float2& ivec, const Float2& nvec );

		static void Refract( const Float2& ivec, const Float2& nvec, float refractionIndex, Float2& result );
		static Float2 Refract( const Float2& ivec, const Float2& nvec, float refractionIndex );

		static void Transform( const Float2& v, const Quaternion& quat, Float2& result );
		static Float2 Transform( const Float2& v, const Quaternion& quat );

		static void Transform( const Float2& v, const Float4x4& m, Float2& result );
		static Float2 Transform( const Float2& v, const Float4x4& m );
		static void Transform( _In_reads_(count) const Float2* varray, size_t count, const Float4x4& m, _Out_writes_(count) Float2* resultArray );

		static void Transform( const Float2& v, const Float4x4& m, Float4& result );
		static void Transform( _In_reads_(count) const Float2* varray, size_t count, const Float4x4& m, _Out_writes_(count) Float4* resultArray );

		static void TransformNormal( const Float2& v, const Float4x4& m, Float2& result );
		static Float2 TransformNormal( const Float2& v, const Float4x4& m );
		static void TransformNormal( _In_reads_(count) const Float2* varray, size_t count, const Float4x4& m, _Out_writes_(count) Float2* resultArray );

	};

	// Binary operators
	Float2 operator+ (const Float2& V1, const Float2& V2);
	Float2 operator- (const Float2& V1, const Float2& V2);
	Float2 operator* (const Float2& V1, const Float2& V2);
	Float2 operator* (const Float2& V, float S);
	Float2 operator/ (const Float2& V1, const Float2& V2);
	Float2 operator* (float S, const Float2& V);


	//------------------------------------------------------------------------------
	// 3D vector
	struct Float3 : public DirectX::XMFLOAT3
	{
		Float3() : XMFLOAT3(0.f, 0.f, 0.f) {}
		explicit Float3(float x) : XMFLOAT3( x, x, x ) {}
		Float3(float _x, float _y, float _z) : XMFLOAT3(_x, _y, _z) {}
		explicit Float3( const float *pArray) : XMFLOAT3(pArray) {}
		Float3(Vector4Param V) { XMStoreFloat3( this, V ); }

		operator Vector4() const { return XMLoadFloat3( this ); }

		// Comparision operators
		bool operator == ( const Float3& V ) const;
		bool operator != ( const Float3& V ) const;

		// Assignment operators
		Float3& operator= (const Float3& V) { x = V.x; y = V.y; z = V.z; return *this; }
		Float3& operator+= (const Float3& V);
		Float3& operator-= (const Float3& V);
		Float3& operator*= (const Float3& V);
		Float3& operator*= (float S);
		Float3& operator/= (float S);

		// Urnary operators
		Float3 operator+ () const { return *this; }
		Float3 operator- () const;

		// Vector operations
		bool InBounds( const Float3& Bounds ) const;

		float Length() const;
		float LengthSquared() const;

		float Dot( const Float3& V ) const;
		void Cross( const Float3& V, Float3& result ) const;
		Float3 Cross( const Float3& V ) const;

		void Normalize();
		void Normalize( Float3& result ) const;

		void Clamp( const Float3& vmin, const Float3& vmax );
		void Clamp( const Float3& vmin, const Float3& vmax, Float3& result ) const;

		// Static functions
		static float Distance( const Float3& v1, const Float3& v2 );
		static float DistanceSquared( const Float3& v1, const Float3& v2 );

		static void Min( const Float3& v1, const Float3& v2, Float3& result );
		static Float3 Min( const Float3& v1, const Float3& v2 );

		static void Max( const Float3& v1, const Float3& v2, Float3& result );
		static Float3 Max( const Float3& v1, const Float3& v2 );

		static void Lerp( const Float3& v1, const Float3& v2, float t, Float3& result );
		static Float3 Lerp( const Float3& v1, const Float3& v2, float t );

		static void SmoothStep( const Float3& v1, const Float3& v2, float t, Float3& result );
		static Float3 SmoothStep( const Float3& v1, const Float3& v2, float t );

		static void Barycentric( const Float3& v1, const Float3& v2, const Float3& v3, float f, float g, Float3& result );
		static Float3 Barycentric( const Float3& v1, const Float3& v2, const Float3& v3, float f, float g );

		static void CatmullRom( const Float3& v1, const Float3& v2, const Float3& v3, const Float3& v4, float t, Float3& result );
		static Float3 CatmullRom( const Float3& v1, const Float3& v2, const Float3& v3, const Float3& v4, float t );

		static void Hermite( const Float3& v1, const Float3& t1, const Float3& v2, const Float3& t2, float t, Float3& result );
		static Float3 Hermite( const Float3& v1, const Float3& t1, const Float3& v2, const Float3& t2, float t );

		static void Reflect( const Float3& ivec, const Float3& nvec, Float3& result );
		static Float3 Reflect( const Float3& ivec, const Float3& nvec );

		static void Refract( const Float3& ivec, const Float3& nvec, float refractionIndex, Float3& result );
		static Float3 Refract( const Float3& ivec, const Float3& nvec, float refractionIndex );

		static void Transform( const Float3& v, const Quaternion& quat, Float3& result );
		static Float3 Transform( const Float3& v, const Quaternion& quat );

		static void Transform( const Float3& v, const Float4x4& m, Float3& result );
		static Float3 Transform( const Float3& v, const Float4x4& m );
		static void Transform( _In_reads_(count) const Float3* varray, size_t count, const Float4x4& m, _Out_writes_(count) Float3* resultArray );

		static void Transform( const Float3& v, const Float4x4& m, Float4& result );
		static void Transform( _In_reads_(count) const Float3* varray, size_t count, const Float4x4& m, _Out_writes_(count) Float4* resultArray );

		static void TransformNormal( const Float3& v, const Float4x4& m, Float3& result );
		static Float3 TransformNormal( const Float3& v, const Float4x4& m );
		static void TransformNormal( _In_reads_(count) const Float3* varray, size_t count, const Float4x4& m, _Out_writes_(count) Float3* resultArray );
	};

	// Binary operators
	Float3 operator+ (const Float3& V1, const Float3& V2);
	Float3 operator- (const Float3& V1, const Float3& V2);
	Float3 operator* (const Float3& V1, const Float3& V2);
	Float3 operator* (const Float3& V, float S);
	Float3 operator/ (const Float3& V1, const Float3& V2);
	Float3 operator* (float S, const Float3& V);
	Float3 operator^ (const Float3& V1, const Float3& V2);

	//------------------------------------------------------------------------------
	// 4D vector
	struct Float4 : public DirectX::XMFLOAT4
	{
		Float4() : XMFLOAT4(0.f, 0.f, 0.f, 0.f) {}
		explicit Float4(float x) : XMFLOAT4( x, x, x, x ) {}
		Float4(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {}
		explicit Float4( const float *pArray) : XMFLOAT4(pArray) {}
		Float4(Vector4Param V) { XMStoreFloat4( this, V ); }

		operator Vector4() const { return XMLoadFloat4( this ); }

		// Comparision operators
		bool operator == ( const Float4& V ) const;
		bool operator != ( const Float4& V ) const;

		// Assignment operators
		Float4& operator= (const Float4& V) { x = V.x; y = V.y; z = V.z; w = V.w; return *this; }
		Float4& operator+= (const Float4& V);
		Float4& operator-= (const Float4& V);
		Float4& operator*= (const Float4& V);
		Float4& operator*= (float S);
		Float4& operator/= (float S);

		// Urnary operators
		Float4 operator+ () const { return *this; }
		Float4 operator- () const;

		// Vector operations
		bool InBounds( const Float4& Bounds ) const;

		float Length() const;
		float LengthSquared() const;

		float Dot( const Float4& V ) const;
		void Cross( const Float4& v1, const Float4& v2, Float4& result ) const;
		Float4 Cross( const Float4& v1, const Float4& v2 ) const;

		void Normalize();
		void Normalize( Float4& result ) const;

		void Clamp( const Float4& vmin, const Float4& vmax );
		void Clamp( const Float4& vmin, const Float4& vmax, Float4& result ) const;

		// Static functions
		static float Distance( const Float4& v1, const Float4& v2 );
		static float DistanceSquared( const Float4& v1, const Float4& v2 );

		static void Min( const Float4& v1, const Float4& v2, Float4& result );
		static Float4 Min( const Float4& v1, const Float4& v2 );

		static void Max( const Float4& v1, const Float4& v2, Float4& result );
		static Float4 Max( const Float4& v1, const Float4& v2 );

		static void Lerp( const Float4& v1, const Float4& v2, float t, Float4& result );
		static Float4 Lerp( const Float4& v1, const Float4& v2, float t );

		static void SmoothStep( const Float4& v1, const Float4& v2, float t, Float4& result );
		static Float4 SmoothStep( const Float4& v1, const Float4& v2, float t );

		static void Barycentric( const Float4& v1, const Float4& v2, const Float4& v3, float f, float g, Float4& result );
		static Float4 Barycentric( const Float4& v1, const Float4& v2, const Float4& v3, float f, float g );

		static void CatmullRom( const Float4& v1, const Float4& v2, const Float4& v3, const Float4& v4, float t, Float4& result );
		static Float4 CatmullRom( const Float4& v1, const Float4& v2, const Float4& v3, const Float4& v4, float t );

		static void Hermite( const Float4& v1, const Float4& t1, const Float4& v2, const Float4& t2, float t, Float4& result );
		static Float4 Hermite( const Float4& v1, const Float4& t1, const Float4& v2, const Float4& t2, float t );

		static void Reflect( const Float4& ivec, const Float4& nvec, Float4& result );
		static Float4 Reflect( const Float4& ivec, const Float4& nvec );

		static void Refract( const Float4& ivec, const Float4& nvec, float refractionIndex, Float4& result );
		static Float4 Refract( const Float4& ivec, const Float4& nvec, float refractionIndex );

		static void Transform( const Float2& v, const Quaternion& quat, Float4& result );
		static Float4 Transform( const Float2& v, const Quaternion& quat );

		static void Transform( const Float3& v, const Quaternion& quat, Float4& result );
		static Float4 Transform( const Float3& v, const Quaternion& quat );

		static void Transform( const Float4& v, const Quaternion& quat, Float4& result );
		static Float4 Transform( const Float4& v, const Quaternion& quat );

		static void Transform( const Float4& v, const Float4x4& m, Float4& result );
		static Float4 Transform( const Float4& v, const Float4x4& m );
		static void Transform( _In_reads_(count) const Float4* varray, size_t count, const Float4x4& m, _Out_writes_(count) Float4* resultArray );
	};

	// Binary operators
	Float4 operator+ (const Float4& V1, const Float4& V2);
	Float4 operator- (const Float4& V1, const Float4& V2);
	Float4 operator* (const Float4& V1, const Float4& V2);
	Float4 operator* (const Float4& V, float S);
	Float4 operator/ (const Float4& V1, const Float4& V2);
	Float4 operator* (float S, const Float4& V);

	//------------------------------------------------------------------------------
	// 4x4 Matrix (assumes right-handed cooordinates)
	struct Float4x4 : public DirectX::XMFLOAT4X4
	{
		Float4x4() : XMFLOAT4X4( 1.f,   0,   0,   0,
			0, 1.f,   0,   0,
			0,   0, 1.f,   0,
			0,   0,   0, 1.f ) {}
		Float4x4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33) : XMFLOAT4X4(m00, m01, m02, m03,
			m10, m11, m12, m13,
			m20, m21, m22, m23,
			m30, m31, m32, m33) {}
		explicit Float4x4( const Float3& r0, const Float3& r1, const Float3& r2 ) : XMFLOAT4X4( r0.x, r0.y, r0.z, 0,
			r1.x, r1.y, r1.z, 0,
			r2.x, r2.y, r2.z, 0,
			0,       0,    0, 1.f ) {}
		explicit Float4x4( const Float4& r0, const Float4& r1, const Float4& r2, const Float4& r3 ) : XMFLOAT4X4( r0.x, r0.y, r0.z, r0.w,
			r1.x, r1.y, r1.z, r1.w,
			r2.x, r2.y, r2.z, r2.w,
			r3.x, r3.y, r3.z, r3.w ) {}
		explicit Float4x4(_In_reads_(16) const float *pArray) : XMFLOAT4X4(pArray) {}
		Float4x4( MatrixParam M ) { XMStoreFloat4x4( this, M ); }

		operator Matrix4x4() const { return XMLoadFloat4x4( this ); }

		// Comparision operators
		bool operator == ( const Float4x4& M ) const;
		bool operator != ( const Float4x4& M ) const;

		// Assignment operators
		Float4x4& operator= (const Float4x4& M) { memcpy_s( this, sizeof(float)*16, &M, sizeof(float)*16); return *this; }
		Float4x4& operator+= (const Float4x4& M);
		Float4x4& operator-= (const Float4x4& M);
		Float4x4& operator*= (const Float4x4& M);
		Float4x4& operator*= (float S);
		Float4x4& operator/= (float S);

		Float4x4& operator/= (const Float4x4& M);
		// Element-wise divide

		// Urnary operators
		Float4x4 operator+ () const { return *this; }
		Float4x4 operator- () const;

		// Properties
		Float3 Up() const { return Float3( _21, _22, _23); }
		void Up( const Float3& v ) { _21 = v.x; _22 = v.y; _23 = v.z; }

		Float3 Down() const { return Float3( -_21, -_22, -_23); }
		void Down( const Float3& v ) { _21 = -v.x; _22 = -v.y; _23 = -v.z; }

		Float3 Right() const { return Float3( _11, _12, _13 ); }
		void Right( const Float3& v ) { _11 = v.x; _12 = v.y; _13 = v.z; }

		Float3 Left() const { return Float3( -_11, -_12, -_13 ); }
		void Left( const Float3& v ) { _11 = -v.x; _12 = -v.y; _13 = -v.z; }

		Float3 Forward() const  { return Float3( -_31, -_32, -_33 ); }
		void Forward( const Float3& v ) { _31 = -v.x; _32 = -v.y; _33 = -v.z; }

		Float3 Backward() const { return Float3( _31, _32, _33 ); }
		void Backward( const Float3& v ) { _31 = v.x; _32 = v.y; _33 = v.z; }

		Float3 Translation() const { return Float3( _41, _42, _43 ); }
		void Translation( const Float3& v ) { _41 = v.x; _42 = v.y; _43 = v.z; }

		// Matrix operations
		bool Decompose( Float3& scale, Quaternion& rotation, Float3& translation );

		Float4x4 Transpose() const;
		void Transpose( Float4x4& result ) const;

		Float4x4 Invert() const;
		void Invert( Float4x4& result ) const;

		float Determinant() const;

		// Static functions
		static Float4x4 Identity();

		static Float4x4 CreateTranslation( const Float3& position );
		static Float4x4 CreateTranslation( float x, float y, float z );

		static Float4x4 CreateScale( const Float3& scales );
		static Float4x4 CreateScale( float xs, float ys, float zs );
		static Float4x4 CreateScale( float scale );

		static Float4x4 CreateRotationX( float radians );
		static Float4x4 CreateRotationY( float radians );
		static Float4x4 CreateRotationZ( float radians );

		static Float4x4 CreateFromAxisAngle( const Float3& axis, float angle );

		static Float4x4 CreatePerspectiveFieldOfView( float fov, float aspectRatio, float nearPlane, float farPlane );
		static Float4x4 CreatePerspective( float width, float height, float nearPlane, float farPlane );
		static Float4x4 CreatePerspectiveOffCenter( float left, float right, float bottom, float top, float nearPlane, float farPlane );
		static Float4x4 CreateOrthographic( float width, float height, float zNearPlane, float zFarPlane );
		static Float4x4 CreateOrthographicOffCenter( float left, float right, float bottom, float top, float zNearPlane, float zFarPlane );

		static Float4x4 CreateLookAt( const Float3& position, const Float3& target, const Float3& up );
		static Float4x4 CreateWorld( const Float3& position, const Float3& forward, const Float3& up );

		static Float4x4 CreateFromQuaternion( const Quaternion& quat );

		static Float4x4 CreateFromYawPitchRoll( float yaw, float pitch, float roll );

		static Float4x4 CreateShadow( const Float3& lightDir, const Plane& plane );

		static Float4x4 CreateReflection( const Plane& plane );

		static void Lerp( const Float4x4& M1, const Float4x4& M2, float t, Float4x4& result );
		static Float4x4 Lerp( const Float4x4& M1, const Float4x4& M2, float t );

		static void Transform( const Float4x4& M, const Quaternion& rotation, Float4x4& result );
		static Float4x4 Transform( const Float4x4& M, const Quaternion& rotation );
	};

	// Binary operators
	Float4x4 operator+ (const Float4x4& M1, const Float4x4& M2);
	Float4x4 operator- (const Float4x4& M1, const Float4x4& M2);
	Float4x4 operator* (const Float4x4& M1, const Float4x4& M2);
	Float4x4 operator* (const Float4x4& M, float S);
	Float4x4 operator/ (const Float4x4& M, float S);
	Float4x4 operator/ (const Float4x4& M1, const Float4x4& M2);
	// Element-wise divide
	Float4x4 operator* (float S, const Float4x4& M);


	//-----------------------------------------------------------------------------
	// Plane
	struct Plane : public DirectX::XMFLOAT4
	{
		Plane() : XMFLOAT4(0.f, 1.f, 0.f, 0.f) {}
		Plane(float _x, float _y, float _z, float _w) : XMFLOAT4(_x, _y, _z, _w) {}
		Plane(const Float3& normal, float d) : XMFLOAT4(normal.x, normal.y, normal.z, d) {}
		Plane(const Float3& point1, const Float3& point2, const Float3& point3 );
		Plane(const Float3& point, const Float3& normal);
		explicit Plane(const Float4& v) : XMFLOAT4(v.x, v.y, v.z, v.w) {}
		explicit Plane( const float *pArray) : XMFLOAT4(pArray) {}
		Plane(Vector4Param V) { XMStoreFloat4( this, V ); }

		operator Vector4() const { return XMLoadFloat4( this ); }

		// Comparision operators
		bool operator == ( const Plane& p ) const;
		bool operator != ( const Plane& p ) const;

		// Assignment operators
		Plane& operator= (const Plane& p) { x = p.x; y = p.y; z = p.z; w = p.w; return *this; }

		// Properties
		Float3 Normal() const { return Float3( x, y, z ); }
		void Normal( const Float3& normal ) { x = normal.x; y = normal.y; z = normal.z; }

		float D() const { return w; }
		void D(float d) { w = d; }

		// Plane operations
		void Normalize();
		void Normalize( Plane& result ) const;

		float Dot( const Float4& v ) const;
		float DotCoordinate( const Float3& position ) const;
		float DotNormal( const Float3& normal ) const;

		// Static functions
		static void Transform( const Plane& plane, const Float4x4& M, Plane& result );
		static Plane Transform( const Plane& plane, const Float4x4& M );

		static void Transform( const Plane& plane, const Quaternion& rotation, Plane& result );
		static Plane Transform( const Plane& plane, const Quaternion& rotation );
		// Input quaternion must be the inverse transpose of the transformation
	};

	//------------------------------------------------------------------------------
	// Quaternion
	struct Quaternion : public DirectX::XMFLOAT4
	{
		Quaternion() : XMFLOAT4(0, 0, 0, 1.f) {}
		Quaternion( float _x, float _y, float _z, float _w ) : XMFLOAT4(_x, _y, _z, _w) {}
		Quaternion( const Float3& v, float scalar ) : XMFLOAT4( v.x, v.y, v.z, scalar ) {}
		explicit Quaternion( const Float4& v ) : XMFLOAT4( v.x, v.y, v.z, v.w ) {}
		explicit Quaternion( const float *pArray) : XMFLOAT4(pArray) {}
		Quaternion(Vector4Param V) { XMStoreFloat4( this, V ); }

		operator Vector4() const { return XMLoadFloat4( this ); }

		// Comparision operators
		bool operator == ( const Quaternion& q ) const;
		bool operator != ( const Quaternion& q ) const;

		// Assignment operators
		Quaternion& operator= (const Quaternion& q) { x = q.x; y = q.y; z = q.z; w = q.w; return *this; }
		Quaternion& operator+= (const Quaternion& q);
		Quaternion& operator-= (const Quaternion& q);
		Quaternion& operator*= (const Quaternion& q);
		Quaternion& operator*= (float S);
		Quaternion& operator/= (const Quaternion& q);

		// Urnary operators
		Quaternion operator+ () const { return *this; }
		Quaternion operator- () const;

		// Quaternion operations
		float Length() const;
		float LengthSquared() const;

		void Normalize();
		void Normalize( Quaternion& result ) const;

		void Conjugate();
		void Conjugate( Quaternion& result ) const;

		void Inverse( Quaternion& result ) const;

		float Dot( const Quaternion& Q ) const;

		// Static functions
		static Quaternion CreateFromAxisAngle( const Float3& axis, float angle );
		static Quaternion CreateFromYawPitchRoll( float yaw, float pitch, float roll );
		static Quaternion CreateFromRotationMatrix( const Float4x4& M );

		static void Lerp( const Quaternion& q1, const Quaternion& q2, float t, Quaternion& result );
		static Quaternion Lerp( const Quaternion& q1, const Quaternion& q2, float t );

		static void Slerp( const Quaternion& q1, const Quaternion& q2, float t, Quaternion& result );
		static Quaternion Slerp( const Quaternion& q1, const Quaternion& q2, float t );

		static void Concatenate( const Quaternion& q1, const Quaternion& q2, Quaternion& result );
		static Quaternion Concatenate( const Quaternion& q1, const Quaternion& q2 );
	};

	// Binary operators
	Quaternion operator+ (const Quaternion& Q1, const Quaternion& Q2);
	Quaternion operator- (const Quaternion& Q1, const Quaternion& Q2);
	Quaternion operator* (const Quaternion& Q1, const Quaternion& Q2);
	Quaternion operator* (const Quaternion& Q, float S);
	Quaternion operator/ (const Quaternion& Q1, const Quaternion& Q2);
	Quaternion operator* (float S, const Quaternion& Q);

	//------------------------------------------------------------------------------
	// Color32
	struct Color32 : public DirectX::XMFLOAT4
	{
		Color32() : XMFLOAT4(0, 0, 0, 1.f) {}
		Color32( float _r, float _g, float _b ) : XMFLOAT4(_r, _g, _b, 1.f) {}
		Color32( float _r, float _g, float _b, float _a ) : XMFLOAT4(_r, _g, _b, _a) {}
		explicit Color32( const Float3& clr ) : XMFLOAT4( clr.x, clr.y, clr.z, 1.f ) {}
		explicit Color32( const Float4& clr ) : XMFLOAT4( clr.x, clr.y, clr.z, clr.w ) {}
		explicit Color32( const float *pArray) : XMFLOAT4(pArray) {}
		Color32(Vector4Param V) { XMStoreFloat4( this, V ); }

		explicit Color32( const Color8& Packed );
		// BGRA Direct3D 9 D3DColor32 packed Color32

		explicit Color32( const Uint8_n4& Packed );
		// RGBA XNA Game Studio packed Color32

		operator Vector4() const { return XMLoadFloat4( this ); }
		operator const float*() const { return reinterpret_cast<const float*>(this); }

		// Comparision operators
		bool operator == ( const Color32& c ) const;
		bool operator != ( const Color32& c ) const;

		// Assignment operators
		Color32& operator= (const Color32& c) { x = c.x; y = c.y; z = c.z; w = c.w; return *this; }
		Color32& operator+= (const Color32& c);
		Color32& operator-= (const Color32& c);
		Color32& operator*= (const Color32& c);
		Color32& operator*= (float S);
		Color32& operator/= (const Color32& c);

		// Urnary operators
		Color32 operator+ () const { return *this; }
		Color32 operator- () const;

		// Properties
		float R() const { return x; }
		void R(float r) { x = r; }

		float G() const { return y; }
		void G(float g) { y = g; }

		float B() const { return z; }
		void B(float b) { z = b; }

		float A() const { return w; }
		void A(float a) { w = a; }

		// Color32 operations
		Color8 BGRA() const;
		Uint8_n4 RGBA() const;

		Float3 ToFloat3() const;
		Float4 ToFloat4() const;

		void Negate();
		void Negate( Color32& result ) const;

		void Saturate();
		void Saturate( Color32& result ) const;

		void Premultiply();
		void Premultiply( Color32& result ) const;

		void AdjustSaturation( float sat );
		void AdjustSaturation( float sat, Color32& result ) const;

		void AdjustContrast( float contrast );
		void AdjustContrast( float contrast, Color32& result ) const;

		// Static functions
		static void Modulate( const Color32& c1, const Color32& c2, Color32& result ); 
		static Color32 Modulate( const Color32& c1, const Color32& c2 ); 

		static void Lerp( const Color32& c1, const Color32& c2, float t, Color32& result );
		static Color32 Lerp( const Color32& c1, const Color32& c2, float t );
	};

	// Binary operators
	Color32 operator+ (const Color32& C1, const Color32& C2);
	Color32 operator- (const Color32& C1, const Color32& C2);
	Color32 operator* (const Color32& C1, const Color32& C2);
	Color32 operator* (const Color32& C, float S);
	Color32 operator/ (const Color32& C1, const Color32& C2);
	Color32 operator* (float S, const Color32& C);

	//------------------------------------------------------------------------------
	// Ray
	class Ray
	{
	public:
		Float3 position;
		Float3 direction;

		Ray() : position(0,0,0), direction(0,0,1) {}
		Ray( const Float3& pos, const Float3& dir ) : position(pos), direction(dir) {}

		// Comparision operators
		bool operator == ( const Ray& r ) const;
		bool operator != ( const Ray& r ) const;

		// Ray operations
		bool Intersects( const DirectX::BoundingSphere& sphere, float& Dist ) const;
		bool Intersects( const DirectX::BoundingBox& box, float& Dist ) const;
		bool Intersects( const Float3& tri0, const Float3& tri1, const Float3& tri2, float& Dist ) const;
		bool Intersects( const Plane& plane, float& Dist ) const;
	};

	class MathHelper
	{
	public:
		static const float Pi;

		inline static float ToRadians(float degress){return degress * (Pi / 180.0f);}
		inline static float ToDegree(float radian){return radian * (180.0f / Pi);}


		inline static bool  NearEqual(float S1, float S2, float Epsilon) {return DirectX::XMScalarNearEqual(S1,S2,Epsilon);}
		inline static float ModAngle(float Value) {return DirectX::XMScalarModAngle(Value);}
			  
		inline static float Sin(float Value) {return DirectX::XMScalarSin(Value);}
		inline static float SinEst(float Value) {return DirectX::XMScalarSinEst(Value);}
		inline static float Cos(float Value) {return DirectX::XMScalarCos(Value);}
		inline static float CosEst(float Value) {return DirectX::XMScalarCosEst(Value);}
		inline static void  SinCos(float* pSin, float* pCos, float Value) {return DirectX::XMScalarSinCos(pSin,pCos,Value);}
		inline static void  SinCosEst(float* pSin, float* pCos, float Value) {return DirectX::XMScalarSinCosEst(pSin,pCos,Value);}
		inline static float ASin(float Value) {return DirectX::XMScalarASin(Value);}
		inline static float ASinEst(float Value) {return DirectX::XMScalarASinEst(Value);}
		inline static float ACos(float Value) {return DirectX::XMScalarACos(Value);}
		inline static float ACosEst(float Value) {return DirectX::XMScalarACosEst(Value);}
	};

	class SimdMath
	{
		inline static bool VerifyCPUSupport(){return DirectX::XMVerifyCPUSupport();}

		//load operation
		inline static Vector4 LoadInt(const uint32_t* pSource) {return DirectX::XMLoadInt(pSource);}
		inline static Vector4 LoadFloat(const float* pSource) {return DirectX::XMLoadFloat(pSource);}

		inline static Vector4 LoadInt2( const uint32_t* pSource) {return DirectX::XMLoadInt2(pSource);}
		inline static Vector4 LoadInt2A( const uint32_t* pSource) {return DirectX::XMLoadInt2A(pSource);}
		inline static Vector4 LoadFloat2(const Float2* pSource) {return DirectX::XMLoadFloat2(pSource);}
		inline static Vector4 LoadFloat2A(const Float2A* pSource) {return DirectX::XMLoadFloat2A(pSource);}
		inline static Vector4 LoadSInt2(const Int2* pSource) {return DirectX::XMLoadSInt2(pSource);}
		inline static Vector4 LoadUInt2(const Uint2* pSource) {return DirectX::XMLoadUInt2(pSource);}

		inline static Vector4 LoadInt3( const uint32_t* pSource) {return DirectX::XMLoadInt3(pSource);}
		inline static Vector4 LoadInt3A( const uint32_t* pSource) {return DirectX::XMLoadInt3A(pSource);}
		inline static Vector4 LoadFloat3(const Float3* pSource) {return DirectX::XMLoadFloat3(pSource);};
		inline static Vector4 LoadFloat3A(const Float3A* pSource) {return DirectX::XMLoadFloat3A((DirectX::XMFLOAT3A*)(pSource));}
		inline static Vector4 LoadSInt3(const Int3* pSource) {return DirectX::XMLoadSInt3(pSource);}
		inline static Vector4 LoadUInt3(const Uint3* pSource) {return DirectX::XMLoadUInt3(pSource);}

		inline static Vector4 LoadInt4( const uint32_t* pSource) {return DirectX::XMLoadInt4(pSource);}
		inline static Vector4 LoadInt4A( const uint32_t* pSource) {return DirectX::XMLoadInt4A(pSource);}
		inline static Vector4 LoadFloat4(const Float4* pSource) {return DirectX::XMLoadFloat4(pSource);}
		inline static Vector4 LoadFloat4A(const Float4A* pSource) {return DirectX::XMLoadFloat4A((DirectX::XMFLOAT4A*)(pSource));}
		inline static Vector4 LoadSInt4(const Int4* pSource) {return DirectX::XMLoadSInt4(pSource);}
		inline static Vector4 LoadUInt4(const Uint4* pSource) {return DirectX::XMLoadUInt4(pSource);}

		inline static Matrix4x4 LoadFloat3x3(const Float3x3* pSource) {return DirectX::XMLoadFloat3x3(pSource);}
		inline static Matrix4x4 LoadFloat4x3(const Float4x3* pSource) {return DirectX::XMLoadFloat4x3(pSource);}
		inline static Matrix4x4 LoadFloat4x3A(const Float4x3A* pSource) {return DirectX::XMLoadFloat4x3A(pSource);}
		inline static Matrix4x4 LoadFloat4x4(const Float4x4* pSource) {return DirectX::XMLoadFloat4x4(pSource);}
		inline static Matrix4x4 LoadFloat4x4A(const Float4x4A* pSource) {return DirectX::XMLoadFloat4x4A(pSource);}

		//Store operations
		inline static void StoreInt(uint32_t* pDestination, Vector4Param V) {return DirectX::XMStoreInt(pDestination,V);}
		inline static void StoreFloat(float* pDestination, Vector4Param V) {return DirectX::XMStoreFloat(pDestination,V);}

		inline static void StoreInt2( uint32_t* pDestination, Vector4Param V) {return DirectX::XMStoreInt2(pDestination, V);}
		inline static void StoreInt2A( uint32_t* pDestination, Vector4Param V) {return DirectX::XMStoreInt2A(pDestination, V);}
		inline static void StoreFloat2(Float2* pDestination, Vector4Param V) {return DirectX::XMStoreFloat2(pDestination,V);}
		inline static void StoreFloat2A(Float2A* pDestination, Vector4Param V) {return DirectX::XMStoreFloat2A(pDestination, V);}
		inline static void StoreSInt2(Int2* pDestination, Vector4Param V) {return DirectX::XMStoreSInt2(pDestination,V);}
		inline static void StoreUInt2(Uint2* pDestination, Vector4Param V) {return DirectX::XMStoreUInt2(pDestination,V);}

		inline static void StoreInt3( uint32_t* pDestination, Vector4Param V) {return DirectX::XMStoreInt3(pDestination,V);}
		inline static void StoreInt3A( uint32_t* pDestination, Vector4Param V) {return DirectX::XMStoreInt3A(pDestination,V);}
		inline static void StoreFloat3(Float3* pDestination, Vector4Param V) {return DirectX::XMStoreFloat3(pDestination,V);}
		inline static void StoreFloat3A(Float3A* pDestination, Vector4Param V) {return DirectX::XMStoreFloat3A((DirectX::XMFLOAT3A*)pDestination,V);}
		inline static void StoreSInt3(Int3* pDestination, Vector4Param V) {return DirectX::XMStoreSInt3(pDestination,V);}
		inline static void StoreUInt3(Uint3* pDestination, Vector4Param V) {return DirectX::XMStoreUInt3(pDestination,V);}

		inline static void StoreInt4( uint32_t* pDestination, Vector4Param V) {return DirectX::XMStoreInt4(pDestination,V);}
		inline static void StoreInt4A( uint32_t* pDestination, Vector4Param V) {return DirectX::XMStoreInt4A(pDestination,V);}
		inline static void StoreFloat4(Float4* pDestination, Vector4Param V) {return DirectX::XMStoreFloat4(pDestination, V);}
		inline static void StoreFloat4A(Float4A* pDestination, Vector4Param V) {return DirectX::XMStoreFloat4A((DirectX::XMFLOAT4A*)pDestination, V);}
		inline static void StoreSInt4(Int4* pDestination, Vector4Param V) {return DirectX::XMStoreSInt4(pDestination,V);}
		inline static void StoreUInt4(Uint4* pDestination, Vector4Param V) {return DirectX::XMStoreUInt4(pDestination,V);}

		inline static void StoreFloat3x3(Float3x3* pDestination, MatrixParam M) {return DirectX::XMStoreFloat3x3(pDestination, M);}
		inline static void StoreFloat4x3(Float4x3* pDestination, MatrixParam M) {return DirectX::XMStoreFloat4x3(pDestination, M);}
		inline static void StoreFloat4x3A(Float4x3A* pDestination, MatrixParam M) {return DirectX::XMStoreFloat4x3A(pDestination, M);}
		inline static void StoreFloat4x4(Float4x4* pDestination, MatrixParam M) {return DirectX::XMStoreFloat4x4(pDestination, M);}
		inline static void StoreFloat4x4A(Float4x4A* pDestination, MatrixParam M) {return DirectX::XMStoreFloat4x4A(pDestination,M);}

		// General vector operations
		inline static Vector4 VectorZero() {return DirectX::XMVectorZero();}
		inline static Vector4 VectorSet(float x, float y, float z, float w) {return DirectX::XMVectorSet(x,y,z,w);}
		inline static Vector4 VectorSetInt(uint32_t x, uint32_t y, uint32_t z, uint32_t w) {return DirectX::XMVectorSetInt(x,y,z,w);}
		inline static Vector4 VectorReplicate(float Value) {return DirectX::XMVectorReplicate(Value);}
		inline static Vector4 VectorReplicatePtr(const float *pValue) {return DirectX::XMVectorReplicatePtr(pValue);}
		inline static Vector4 VectorReplicateInt(uint32_t Value)  {return DirectX::XMVectorReplicateInt(Value);}
		inline static Vector4 VectorReplicateIntPtr(const uint32_t *pValue)  {return DirectX::XMVectorReplicateIntPtr(pValue);}
		inline static Vector4 VectorTrueInt()  {return DirectX::XMVectorTrueInt();}
		inline static Vector4 VectorFalseInt()  {return DirectX::XMVectorFalseInt();}
		inline static Vector4 VectorSplatX(Vector4Param V)  {return DirectX::XMVectorSplatX(V);}
		inline static Vector4 VectorSplatY(Vector4Param V)  {return DirectX::XMVectorSplatY(V);}
		inline static Vector4 VectorSplatZ(Vector4Param V)  {return DirectX::XMVectorSplatZ(V);}
		inline static Vector4 VectorSplatW(Vector4Param V)  {return DirectX::XMVectorSplatW(V);}
		inline static Vector4 VectorSplatOne()  {return DirectX::XMVectorSplatOne();}
		inline static Vector4 VectorSplatInfinity()  {return DirectX::XMVectorSplatInfinity();}
		inline static Vector4 VectorSplatQNaN()  {return DirectX::XMVectorSplatQNaN();}
		inline static Vector4 VectorSplatEpsilon()  {return DirectX::XMVectorSplatEpsilon();}
		inline static Vector4 VectorSplatSignMask()  {return DirectX::XMVectorSplatSignMask();}

		inline static float VectorGetByIndex(Vector4Param V, size_t i){return DirectX::XMVectorGetByIndex(V,i);}
		inline static float VectorGetX(Vector4Param V){return DirectX::XMVectorGetX(V);}
		inline static float VectorGetY(Vector4Param V){return DirectX::XMVectorGetY(V);}
		inline static float VectorGetZ(Vector4Param V){return DirectX::XMVectorGetZ(V);}
		inline static float VectorGetW(Vector4Param V){return DirectX::XMVectorGetW(V);}
			 
		inline static void VectorGetByIndexPtr(float *f, Vector4Param V, size_t i)  {return DirectX::XMVectorGetByIndexPtr(f,V,i);}
		inline static void VectorGetXPtr(float *x, Vector4Param V)  {return DirectX::XMVectorGetXPtr(x , V);}
		inline static void VectorGetYPtr(float *y, Vector4Param V)  {return DirectX::XMVectorGetYPtr(y , V);}
		inline static void VectorGetZPtr(float *z, Vector4Param V)  {return DirectX::XMVectorGetZPtr(z , V);}
		inline static void VectorGetWPtr(float *w, Vector4Param V)  {return DirectX::XMVectorGetWPtr(w , V);}

		inline static uint32_t VectorGetIntByIndex(Vector4Param V, size_t i){return DirectX::XMVectorGetIntByIndex(V,i);}
		inline static uint32_t VectorGetIntX(Vector4Param V) {return DirectX::XMVectorGetIntX(V);}
		inline static uint32_t VectorGetIntY(Vector4Param V) {return DirectX::XMVectorGetIntY(V);}
		inline static uint32_t VectorGetIntZ(Vector4Param V) {return DirectX::XMVectorGetIntZ(V);}
		inline static uint32_t VectorGetIntW(Vector4Param V) {return DirectX::XMVectorGetIntW(V);}

		inline static void VectorGetIntByIndexPtr(uint32_t *x, Vector4Param V, size_t i){return DirectX::XMVectorGetIntByIndexPtr(x,V,i);}
		inline static void VectorGetIntXPtr(uint32_t *x, Vector4Param V) {return DirectX::XMVectorGetIntXPtr(x,V);}
		inline static void VectorGetIntYPtr(uint32_t *y, Vector4Param V) {return DirectX::XMVectorGetIntYPtr(y,V);}
		inline static void VectorGetIntZPtr(uint32_t *z, Vector4Param V) {return DirectX::XMVectorGetIntZPtr(z,V);}
		inline static void VectorGetIntWPtr(uint32_t *w, Vector4Param V) {return DirectX::XMVectorGetIntWPtr(w,V);}

		inline static Vector4 VectorSetByIndex(Vector4Param V,float f, size_t i) {return DirectX::XMVectorSetByIndex(V,f,i);}
		inline static Vector4 VectorSetX(Vector4Param V, float x) {return DirectX::XMVectorSetX(V,x);}
		inline static Vector4 VectorSetY(Vector4Param V, float y) {return DirectX::XMVectorSetY(V,y);}
		inline static Vector4 VectorSetZ(Vector4Param V, float z) {return DirectX::XMVectorSetZ(V,z);}
		inline static Vector4 VectorSetW(Vector4Param V, float w) {return DirectX::XMVectorSetW(V,w);}
			    
		inline static Vector4 VectorSetByIndexPtr(Vector4Param V, const float *f, size_t i) {return DirectX::XMVectorSetByIndexPtr(V,f,i);}
		inline static Vector4 VectorSetXPtr(Vector4Param V, const float *x) {return DirectX::XMVectorSetXPtr(V,x);}
		inline static Vector4 VectorSetYPtr(Vector4Param V, const float *y) {return DirectX::XMVectorSetYPtr(V,y);}
		inline static Vector4 VectorSetZPtr(Vector4Param V, const float *z) {return DirectX::XMVectorSetZPtr(V,z);}
		inline static Vector4 VectorSetWPtr(Vector4Param V, const float *w) {return DirectX::XMVectorSetWPtr(V,w);}
			    
		inline static Vector4 VectorSetIntByIndex(Vector4Param V, uint32_t x, size_t i) {return DirectX::XMVectorSetIntByIndex(V,x,i);}
		inline static Vector4 VectorSetIntX(Vector4Param V, uint32_t x) {return DirectX::XMVectorSetIntX(V,x);}
		inline static Vector4 VectorSetIntY(Vector4Param V, uint32_t y) {return DirectX::XMVectorSetIntY(V,y);}
		inline static Vector4 VectorSetIntZ(Vector4Param V, uint32_t z) {return DirectX::XMVectorSetIntZ(V,z);}
		inline static Vector4 VectorSetIntW(Vector4Param V, uint32_t w) {return DirectX::XMVectorSetIntW(V,w);}
			    
		inline static Vector4 VectorSetIntByIndexPtr(Vector4Param V, const uint32_t *x, size_t i) {return DirectX::XMVectorSetIntByIndexPtr(V,x,i);}
		inline static Vector4 VectorSetIntXPtr(Vector4Param V, const uint32_t *x) {return DirectX::XMVectorSetIntXPtr(V,x);}
		inline static Vector4 VectorSetIntYPtr(Vector4Param V, const uint32_t *y) {return DirectX::XMVectorSetIntYPtr(V,y);}
		inline static Vector4 VectorSetIntZPtr(Vector4Param V, const uint32_t *z) {return DirectX::XMVectorSetIntZPtr(V,z);}
		inline static Vector4 VectorSetIntWPtr(Vector4Param V, const uint32_t *w) {return DirectX::XMVectorSetIntWPtr(V,w);}


		inline static Vector4 VectorSwizzle(Vector4Param V, uint32_t E0, uint32_t E1, uint32_t E2, uint32_t E3) {return DirectX::XMVectorSwizzle(V,E0,E1,E2,E3);}
		inline static Vector4 VectorPermute(Vector4Param V1, Vector4Param V2, uint32_t PermuteX, uint32_t PermuteY, uint32_t PermuteZ, uint32_t PermuteW)
		{
			return DirectX::XMVectorPermute(V1,V2,PermuteX,PermuteY,PermuteZ,PermuteW);
		}
		inline static Vector4 VectorSelectControl(uint32_t VectorIndex0, uint32_t VectorIndex1, uint32_t VectorIndex2, uint32_t VectorIndex3)
		{
			return DirectX::XMVectorSelectControl(VectorIndex0,VectorIndex1,VectorIndex2,VectorIndex3);
		}
		inline static Vector4 VectorSelect(Vector4Param V1, Vector4Param V2, Vector4Param Control){return DirectX::XMVectorSelect(V1,V2,Control);}
		inline static Vector4 VectorMergeXY(Vector4Param V1, Vector4Param V2){return DirectX::XMVectorMergeXY(V1,V2);}
		inline static Vector4 VectorMergeZW(Vector4Param V1, Vector4Param V2){return DirectX::XMVectorMergeZW(V1,V2);}


		inline static Vector4 XMVectorShiftLeft(Vector4Param V1, Vector4Param V2, uint32_t Elements){return DirectX::XMVectorShiftLeft(V1,V2,Elements);}
		inline static Vector4 XMVectorRotateLeft(Vector4Param V, uint32_t Elements){return DirectX::XMVectorRotateLeft(V,Elements);}
		inline static Vector4 XMVectorRotateRight(Vector4Param V, uint32_t Elements){return DirectX::XMVectorRotateRight(V,Elements);}
		inline static Vector4 XMVectorInsert(Vector4Param VD, Vector4Param VS, uint32_t VSLeftRotateElements,
			uint32_t Select0, uint32_t Select1, uint32_t Select2, uint32_t Select3)
		{
			return DirectX::XMVectorInsert(VD,VS,VSLeftRotateElements,Select0,Select1,Select2,Select3);
		}

		inline static Vector4 VectorEqual(Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorEqual(V1,V2);}
		inline static Vector4 VectorEqualR(uint32_t* pCR, Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorEqualR(pCR,V1,V2);}
		inline static Vector4 VectorEqualInt(Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorEqualInt(V1,V2);}
		inline static Vector4 VectorEqualIntR(uint32_t* pCR, Vector4Param V, Vector4Param V2) {return DirectX::XMVectorEqualIntR(pCR,V,V2);}
		inline static Vector4 VectorNearEqual(Vector4Param V1, Vector4Param V2, Vector4Param Epsilon) {return DirectX::XMVectorNearEqual(V1,V2,Epsilon);}
		inline static Vector4 VectorNotEqual(Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorNotEqual(V1,V2);}
		inline static Vector4 VectorNotEqualInt(Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorNotEqualInt(V1,V2);}
		inline static Vector4 VectorGreater(Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorGreater(V1,V2);}
		inline static Vector4 VectorGreaterR(uint32_t* pCR, Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorGreaterR(pCR,V1,V2);}
		inline static Vector4 VectorGreaterOrEqual(Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorGreaterOrEqual(V1,V2);}
		inline static Vector4 VectorGreaterOrEqualR(uint32_t* pCR, Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorGreaterOrEqualR(pCR,V1,V2);}
		inline static Vector4 VectorLess(Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorLess(V1,V2);}
		inline static Vector4 VectorLessOrEqual(Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorLessOrEqual(V1,V2);}
		inline static Vector4 VectorInBounds(Vector4Param V, Vector4Param Bounds) {return DirectX::XMVectorInBounds(V,Bounds);}
		inline static Vector4 VectorInBoundsR(uint32_t* pCR, Vector4Param V, Vector4Param Bounds) {return DirectX::XMVectorInBoundsR(pCR,V,Bounds);}

		inline static Vector4 VectorIsNaN(Vector4Param V) {return DirectX::XMVectorIsNaN(V);}
		inline static Vector4 VectorIsInfinite(Vector4Param V) {return DirectX::XMVectorIsInfinite(V);}
			    
		inline static Vector4 VectorMin(Vector4Param V1,Vector4Param V2) {return DirectX::XMVectorMin(V1,V2);}
		inline static Vector4 VectorMax(Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorMax(V1,V2);}
		inline static Vector4 VectorRound(Vector4Param V) {return DirectX::XMVectorRound(V);}
		inline static Vector4 VectorTruncate(Vector4Param V) {return DirectX::XMVectorTruncate(V);}
		inline static Vector4 VectorFloor(Vector4Param V) {return DirectX::XMVectorFloor(V);}
		inline static Vector4 VectorCeiling(Vector4Param V) {return DirectX::XMVectorCeiling(V);}
		inline static Vector4 VectorClamp(Vector4Param V, Vector4Param Min, Vector4Param Max) {return DirectX::XMVectorClamp(V,Min,Max);}
		inline static Vector4 VectorSaturate(Vector4Param V) {return DirectX::XMVectorSaturate(V);}

		inline static Vector4 VectorAndInt(Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorAndInt(V1,V2);}
		inline static Vector4 VectorAndCInt(Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorAndCInt(V1,V2);}
		inline static Vector4 VectorOrInt(Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorOrInt(V1,V2);}
		inline static Vector4 VectorNorInt(Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorNorInt(V1,V2);}
		inline static Vector4 VectorXorInt(Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorXorInt(V1,V2);}

		inline static Vector4 VectorNegate(Vector4Param V) {return DirectX::XMVectorNegate(V);}
		inline static Vector4 VectorAdd(Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorAdd(V1,V2);}
		inline static Vector4 VectorAddAngles(Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorAddAngles(V1,V2);}
		inline static Vector4 VectorSubtract(Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorSubtract(V1,V2);}
		inline static Vector4 VectorSubtractAngles(Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorSubtractAngles(V1,V2);}
		inline static Vector4 VectorMultiply(Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorMultiply(V1,V2);}
		inline static Vector4 VectorMultiplyAdd(Vector4Param V1, Vector4Param V2, Vector4Param V3) {return DirectX::XMVectorMultiplyAdd(V1,V2,V3);}
		inline static Vector4 VectorDivide(Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorDivide(V1,V2);}
		inline static Vector4 VectorNegativeMultiplySubtract(Vector4Param V1, Vector4Param V2, Vector4Param V3) {return DirectX::XMVectorNegativeMultiplySubtract(V1,V2,V3);}
		inline static Vector4 VectorScale(Vector4Param V, float ScaleFactor) {return DirectX::XMVectorScale(V,ScaleFactor);}
		inline static Vector4 VectorReciprocalEst(Vector4Param V) {return DirectX::XMVectorReciprocalEst(V);}
		inline static Vector4 VectorReciprocal(Vector4Param V) {return DirectX::XMVectorReciprocal(V);}
		inline static Vector4 VectorSqrtEst(Vector4Param V) {return DirectX::XMVectorSqrtEst(V);}
		inline static Vector4 VectorSqrt(Vector4Param V) {return DirectX::XMVectorSqrt(V);}
		inline static Vector4 VectorReciprocalSqrtEst(Vector4Param V) {return DirectX::XMVectorReciprocalSqrtEst(V);}
		inline static Vector4 VectorReciprocalSqrt(Vector4Param V) {return DirectX::XMVectorReciprocalSqrt(V);}
		inline static Vector4 VectorExp(Vector4Param V) {return DirectX::XMVectorExp(V);}
		inline static Vector4 VectorLog(Vector4Param V) {return DirectX::XMVectorLog(V);}
		inline static Vector4 VectorPow(Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorPow(V1	,V2);}
		inline static Vector4 VectorAbs(Vector4Param V) {return DirectX::XMVectorAbs(V);}
		inline static Vector4 VectorMod(Vector4Param V1, Vector4Param V2) {return DirectX::XMVectorMod(V1,V2);}
		inline static Vector4 VectorModAngles(Vector4Param Angles) {return DirectX::XMVectorModAngles(Angles);}
		inline static Vector4 VectorSin(Vector4Param V) {return DirectX::XMVectorSin(V);}
		inline static Vector4 VectorSinEst(Vector4Param V) {return DirectX::XMVectorSinEst(V);}
		inline static Vector4 VectorCos(Vector4Param V) {return DirectX::XMVectorCos(V);}
		inline static Vector4 VectorCosEst(Vector4Param V) {return DirectX::XMVectorCosEst(V);}
		inline static void    VectorSinCos(Vector4* pSin, Vector4* pCos, Vector4Param V) {return DirectX::XMVectorSinCos(pSin,pCos,V);}
		inline static void    VectorSinCosEst(Vector4* pSin, Vector4* pCos, Vector4Param V) {return DirectX::XMVectorSinCosEst(pSin,pCos,V);}
		inline static Vector4 VectorTan(Vector4Param V) {return DirectX::XMVectorTan(V);}
		inline static Vector4 VectorTanEst(Vector4Param V) {return DirectX::XMVectorTanEst(V);}
		inline static Vector4 VectorSinH(Vector4Param V) {return DirectX::XMVectorSinH(V);}
		inline static Vector4 VectorCosH(Vector4Param V) {return DirectX::XMVectorCosH(V);}
		inline static Vector4 VectorTanH(Vector4Param V) {return DirectX::XMVectorTanH(V);}
		inline static Vector4 VectorASin(Vector4Param V) {return DirectX::XMVectorASin(V);}
		inline static Vector4 VectorASinEst(Vector4Param V) {return DirectX::XMVectorASinEst(V);}
		inline static Vector4 VectorACos(Vector4Param V) {return DirectX::XMVectorACos(V);}
		inline static Vector4 VectorACosEst(Vector4Param V) {return DirectX::XMVectorACosEst(V);}
		inline static Vector4 VectorATan(Vector4Param V) {return DirectX::XMVectorATan(V);}
		inline static Vector4 VectorATanEst(Vector4Param V) {return DirectX::XMVectorATanEst(V);}
		inline static Vector4 VectorATan2(Vector4Param Y, Vector4Param X) {return DirectX::XMVectorATan2(Y,X);}
		inline static Vector4 VectorATan2Est(Vector4Param Y, Vector4Param X) {return DirectX::XMVectorATan2Est(Y,X);}
		inline static Vector4 VectorLerp(Vector4Param V0, Vector4Param V1, float t) {return DirectX::XMVectorLerp(V0,V1,t);}
		inline static Vector4 VectorLerpV(Vector4Param V0, Vector4Param V1, Vector4Param T) {return DirectX::XMVectorLerpV(V0,V1,T);}
		inline static Vector4 VectorHermite(Vector4Param Position0, Vector4Param Tangent0, Vector4Param Position1, Vector4Param4 Tangent1, float t) 
		{
			return DirectX::XMVectorHermite(Position0,Tangent0,Position1,Tangent1,t);
		}
		inline static Vector4 VectorHermiteV(Vector4Param Position0, Vector4Param Tangent0, Vector4Param Position1, Vector4Param4 Tangent1, Vector4ParamN T) 
		{
			return DirectX::XMVectorHermiteV(Position0,Tangent0,Position1,Tangent1,T);
		}
		inline static Vector4 VectorCatmullRom(Vector4Param Position0, Vector4Param Position1, Vector4Param Position2, Vector4Param4 Position3, float t) 
		{
			return DirectX::XMVectorCatmullRom(Position0,Position1,Position2,Position3,t);
		}
		inline static Vector4 VectorCatmullRomV(Vector4Param Position0, Vector4Param Position1, Vector4Param Position2, Vector4Param4 Position3, Vector4ParamN T) 
		{
			return DirectX::XMVectorCatmullRomV(Position0,Position1,Position2,Position3,T);
		}
		inline static Vector4 VectorBaryCentric(Vector4Param Position0, Vector4Param Position1, Vector4Param Position2, float f, float g) 
		{
			return DirectX::XMVectorBaryCentric(Position0,Position1,Position2,f,g);
		}
		inline static Vector4 VectorBaryCentricV(Vector4Param Position0, Vector4Param Position1, Vector4Param Position2, Vector4Param4 F, Vector4ParamN G) 
		{
			return DirectX::XMVectorBaryCentricV(Position0,Position1,Position2,F,G);
		}

		//////////////////////////////////////////////////////////////////////////
		//2D vector operations
		inline static bool     Vector2Equal(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector2Equal(V1,V2);}
		inline static uint32_t Vector2EqualR(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector2EqualR(V1,V2);}
		inline static bool     Vector2EqualInt(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector2EqualInt(V1,V2);}
		inline static uint32_t Vector2EqualIntR(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector2EqualIntR(V1,V2);}
		inline static bool     Vector2NearEqual(Vector4Param V1, Vector4Param V2, Vector4Param Epsilon) {return DirectX::XMVector2NearEqual(V1,V2,Epsilon);}
		inline static bool     Vector2NotEqual(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector2NotEqual(V1,V2);}
		inline static bool     Vector2NotEqualInt(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector2NotEqualInt(V1,V2);}
		inline static bool     Vector2Greater(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector2Greater(V1,V2);}
		inline static uint32_t Vector2GreaterR(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector2GreaterR(V1,V2);}
		inline static bool     Vector2GreaterOrEqual(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector2GreaterOrEqual(V1,V2);}
		inline static uint32_t Vector2GreaterOrEqualR(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector2GreaterOrEqualR(V1,V2);}
		inline static bool     Vector2Less(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector2Less(V1,V2);}
		inline static bool     Vector2LessOrEqual(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector2LessOrEqual(V1,V2);}
		inline static bool     Vector2InBounds(Vector4Param V, Vector4Param Bounds) {return DirectX::XMVector2InBounds(V,Bounds);}

		inline static bool MVector2IsNaN(Vector4Param V) {return DirectX::XMVector2IsNaN(V);}
		inline static bool MVector2IsInfinite(Vector4Param V) {return DirectX::XMVector2IsInfinite(V);}

		inline static Vector4 Vector2Dot(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector2Dot(V1,V2);}
		inline static Vector4 Vector2Cross(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector2Cross(V1,V2);}
		inline static Vector4 Vector2LengthSq(Vector4Param V) {return DirectX::XMVector2LengthSq(V);}
		inline static Vector4 Vector2ReciprocalLengthEst(Vector4Param V) {return DirectX::XMVector2ReciprocalLengthEst(V);}
		inline static Vector4 Vector2ReciprocalLength(Vector4Param V) {return DirectX::XMVector2ReciprocalLength(V);}
		inline static Vector4 Vector2LengthEst(Vector4Param V) {return DirectX::XMVector2LengthEst(V);}
		inline static Vector4 Vector2Length(Vector4Param V) {return DirectX::XMVector2Length(V);}
		inline static Vector4 Vector2NormalizeEst(Vector4Param V) {return DirectX::XMVector2NormalizeEst(V);}
		inline static Vector4 Vector2Normalize(Vector4Param V) {return DirectX::XMVector2Normalize(V);}
		inline static Vector4 Vector2ClampLength(Vector4Param V, float LengthMin, float LengthMax) {return DirectX::XMVector2ClampLength(V,LengthMin,LengthMax);}
		inline static Vector4 Vector2ClampLengthV(Vector4Param V, Vector4Param LengthMin, Vector4Param LengthMax) {return DirectX::XMVector2ClampLengthV(V,LengthMin,LengthMax);}
		inline static Vector4 Vector2Reflect(Vector4Param Incident, Vector4Param Normal) {return DirectX::XMVector2Reflect(Incident,Normal);}
		inline static Vector4 Vector2Refract(Vector4Param Incident, Vector4Param Normal, float RefractionIndex) {return DirectX::XMVector2Refract(Incident,Normal,RefractionIndex);}
		inline static Vector4 Vector2RefractV(Vector4Param Incident, Vector4Param Normal, Vector4Param RefractionIndex) {return DirectX::XMVector2RefractV(Incident,Normal,RefractionIndex);}
		inline static Vector4 Vector2Orthogonal(Vector4Param V) {return DirectX::XMVector2Orthogonal(V);}
		inline static Vector4 Vector2AngleBetweenNormalsEst(Vector4Param N1, Vector4Param N2) {return DirectX::XMVector2AngleBetweenNormalsEst(N1,N2);}
		inline static Vector4 Vector2AngleBetweenNormals(Vector4Param N1, Vector4Param N2) {return DirectX::XMVector2AngleBetweenNormals(N1,N2);}
		inline static Vector4 Vector2AngleBetweenVectors(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector2AngleBetweenVectors(V1,V2);}
		inline static Vector4 Vector2LinePointDistance(Vector4Param LinePoint1, Vector4Param LinePoint2, Vector4Param Point) {return DirectX::XMVector2LinePointDistance(LinePoint1,LinePoint2,Point);}
		inline static Vector4 Vector2IntersectLine(Vector4Param Line1Point1, Vector4Param Line1Point2, Vector4Param Line2Point1, Vector4Param4 Line2Point2) 
		{
			return DirectX::XMVector2IntersectLine(Line1Point1,Line1Point2,Line2Point1,Line2Point2);
		}
		inline static Vector4 Vector2Transform(Vector4Param V, MatrixParam M) {return DirectX::XMVector2Transform(V,M);}
		inline static Float4* Vector2TransformStream(Float4* pOutputStream,size_t OutputStride,const Float2* pInputStream,size_t InputStride, size_t VectorCount, MatrixParam M) 
		{
			return (Float4*)DirectX::XMVector2TransformStream(pOutputStream,OutputStride,pInputStream,InputStride,VectorCount,M);
		}
		inline static Vector4 Vector2TransformCoord(Vector4Param V, MatrixParam M) {return DirectX::XMVector2TransformCoord(V,M);}
		inline static Float2* Vector2TransformCoordStream(Float2* pOutputStream,size_t OutputStride,const Float2* pInputStream,size_t InputStride, size_t VectorCount, MatrixParam M) 
		{
			return (Float2*)DirectX::XMVector2TransformCoordStream(pOutputStream,OutputStride,pInputStream,InputStride,VectorCount,M);
		}
		inline static Vector4 Vector2TransformNormal(Vector4Param V, MatrixParam M) 
		{
			return DirectX::XMVector2TransformNormal(V,M);
		}
		inline static Float2* Vector2TransformNormalStream(Float2* pOutputStream,size_t OutputStride,const Float2* pInputStream,size_t InputStride, size_t VectorCount, MatrixParam M) 
		{
			return (Float2*)DirectX::XMVector2TransformNormalStream(pOutputStream,OutputStride,pInputStream,InputStride,VectorCount,M);
		}

		//////////////////////////////////////////////////////////////////////////
		//3D vector operations
		inline static bool     XMVector3Equal(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector3Equal(V1,V2);}
		inline static uint32_t XMVector3EqualR(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector3EqualR(V1,V2);}
		inline static bool     XMVector3EqualInt(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector3EqualInt(V1,V2);}
		inline static uint32_t XMVector3EqualIntR(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector3EqualIntR(V1,V2);}
		inline static bool     XMVector3NearEqual(Vector4Param V1, Vector4Param V2, Vector4Param Epsilon) {return DirectX::XMVector3NearEqual(V1,V2,Epsilon);}
		inline static bool     XMVector3NotEqual(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector3NotEqual(V1,V2);}
		inline static bool     XMVector3NotEqualInt(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector3NotEqualInt(V1,V2);}
		inline static bool     XMVector3Greater(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector3Greater(V1,V2);}
		inline static uint32_t XMVector3GreaterR(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector3GreaterR(V1,V2);}
		inline static bool     XMVector3GreaterOrEqual(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector3GreaterOrEqual(V1,V2);}
		inline static uint32_t XMVector3GreaterOrEqualR(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector3GreaterOrEqualR(V1,V2);}
		inline static bool     XMVector3Less(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector3Less(V1,V2);}
		inline static bool     XMVector3LessOrEqual(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector3LessOrEqual(V1,V2);}
		inline static bool     XMVector3InBounds(Vector4Param V, Vector4Param Bounds) {return DirectX::XMVector3InBounds(V,Bounds);}
		inline static bool     XMVector3IsNaN(Vector4Param V) {return DirectX::XMVector3IsNaN(V);}
		inline static bool     XMVector3IsInfinite(Vector4Param V) {return DirectX::XMVector3IsInfinite(V);}

		inline static Vector4 Vector3Dot(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector3Dot(V1,V2);}
		inline static Vector4 Vector3Cross(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector3Cross(V1,V2);}
		inline static Vector4 Vector3LengthSq(Vector4Param V) {return DirectX::XMVector3LengthSq(V);}
		inline static Vector4 Vector3ReciprocalLengthEst(Vector4Param V) {return DirectX::XMVector3ReciprocalLengthEst(V);}
		inline static Vector4 Vector3ReciprocalLength(Vector4Param V) {return DirectX::XMVector3ReciprocalLength(V);}
		inline static Vector4 Vector3LengthEst(Vector4Param V) {return DirectX::XMVector3LengthEst(V);}
		inline static Vector4 Vector3Length(Vector4Param V) {return DirectX::XMVector3Length(V);}
		inline static Vector4 Vector3NormalizeEst(Vector4Param V) {return DirectX::XMVector3NormalizeEst(V);}
		inline static Vector4 Vector3Normalize(Vector4Param V) {return DirectX::XMVector3Normalize(V);}
		inline static Vector4 Vector3ClampLength(Vector4Param V, float LengthMin, float LengthMax) {return DirectX::XMVector3ClampLength(V,LengthMin,LengthMax);}
		inline static Vector4 Vector3ClampLengthV(Vector4Param V, Vector4Param LengthMin, Vector4Param LengthMax) {return DirectX::XMVector3ClampLengthV(V,LengthMin,LengthMax);}
		inline static Vector4 Vector3Reflect(Vector4Param Incident, Vector4Param Normal) {return DirectX::XMVector3Reflect(Incident,Normal);}
		inline static Vector4 Vector3Refract(Vector4Param Incident, Vector4Param Normal, float RefractionIndex) {return DirectX::XMVector3Refract(Incident,Normal,RefractionIndex);}
		inline static Vector4 Vector3RefractV(Vector4Param Incident, Vector4Param Normal, Vector4Param RefractionIndex) {return DirectX::XMVector3RefractV(Incident,Normal,RefractionIndex);}
		inline static Vector4 Vector3Orthogonal(Vector4Param V) {return DirectX::XMVector3Orthogonal(V);}
		inline static Vector4 Vector3AngleBetweenNormalsEst(Vector4Param N1, Vector4Param N2) {return DirectX::XMVector3AngleBetweenNormalsEst(N1,N2);}
		inline static Vector4 Vector3AngleBetweenNormals(Vector4Param N1, Vector4Param N2) {return DirectX::XMVector3AngleBetweenNormals(N1,N2);}
		inline static Vector4 Vector3AngleBetweenVectors(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector3AngleBetweenVectors(V1,V2);}
		inline static Vector4 Vector3LinePointDistance(Vector4Param LinePoint1, Vector4Param LinePoint2, Vector4Param Point) {return DirectX::XMVector3LinePointDistance(LinePoint1,LinePoint2,Point);}
		inline static void    Vector3ComponentsFromNormal(Vector4* pParallel, Vector4* pPerpendicular, Vector4Param V, Vector4Param Normal) {return DirectX::XMVector3ComponentsFromNormal(pParallel,pPerpendicular,V,Normal);}
		inline static Vector4 Vector3Rotate(Vector4Param V, Vector4Param RotationQuaternion) {return DirectX::XMVector3Rotate(V,RotationQuaternion);}
		inline static Vector4 Vector3InverseRotate(Vector4Param V, Vector4Param RotationQuaternion) {return DirectX::XMVector3InverseRotate(V,RotationQuaternion);}
		inline static Vector4 Vector3Transform(Vector4Param V, MatrixParam M) {return DirectX::XMVector3Transform(V,M);}
		inline static Float4* Vector3TransformStream(Float4* pOutputStream,size_t OutputStride,const Float3* pInputStream,size_t InputStride, size_t VectorCount, MatrixParam M) 
		{
			return (Float4*)DirectX::XMVector3TransformStream(pOutputStream,OutputStride,pInputStream,InputStride,VectorCount,M);
		}
		inline static Vector4 Vector3TransformCoord(Vector4Param V, MatrixParam M) {return DirectX::XMVector3TransformCoord(V,M);}
		inline static Float3* Vector3TransformCoordStream(Float3* pOutputStream,size_t OutputStride,const Float3* pInputStream,size_t InputStride, size_t VectorCount, MatrixParam M) 
		{
			return (Float3*)DirectX::XMVector3TransformCoordStream(pOutputStream,OutputStride,pInputStream,InputStride,VectorCount,M);
		}
		inline static Vector4 Vector3TransformNormal(Vector4Param V, MatrixParam M) {return DirectX::XMVector3TransformNormal(V,M);}
		inline static Float3* Vector3TransformNormalStream(Float3* pOutputStream,size_t OutputStride,const Float3* pInputStream,size_t InputStride, size_t VectorCount, MatrixParam M) 
		{
			return (Float3*)DirectX::XMVector3TransformNormalStream(pOutputStream,OutputStride,pInputStream,InputStride,VectorCount,M);
		}
		inline static Vector4 Vector3Project(Vector4Param V, float ViewportX, float ViewportY, float ViewportWidth, float ViewportHeight, float ViewportMinZ, float ViewportMaxZ, 
			MatrixParam Projection, MatrixParam View, MatrixParam World) 
		{
			return DirectX::XMVector3Project(V,ViewportX,ViewportY,ViewportWidth,ViewportHeight,ViewportMinZ,ViewportMaxZ,Projection,View,World);
		}
		inline static Float3* Vector3ProjectStream(Float3* pOutputStream,size_t OutputStride,const Float3* pInputStream,size_t InputStride, size_t VectorCount, 
									   float ViewportX, float ViewportY, float ViewportWidth, float ViewportHeight, float ViewportMinZ, float ViewportMaxZ, 
									   MatrixParam Projection, MatrixParam View, MatrixParam World) 
		{
			return (Float3*)DirectX::XMVector3ProjectStream(pOutputStream,OutputStride,pInputStream,InputStride,VectorCount,ViewportX,ViewportY,ViewportWidth,
				ViewportHeight,ViewportMinZ,ViewportMaxZ,Projection,View,World);
		}
		inline static Vector4 Vector3Unproject(Vector4Param V, float ViewportX, float ViewportY, float ViewportWidth, float ViewportHeight, float ViewportMinZ, float ViewportMaxZ, 
								   MatrixParam Projection, MatrixParam View, MatrixParam World) 
		{
			return DirectX::XMVector3Unproject(V,ViewportX,ViewportY,ViewportWidth,ViewportHeight,ViewportMinZ,ViewportMaxZ,Projection,View,World);
		}
		inline static Float3* Vector3UnprojectStream(Float3* pOutputStream,size_t OutputStride,const Float3* pInputStream,size_t InputStride, size_t VectorCount, 
												 float ViewportX, float ViewportY, float ViewportWidth, float ViewportHeight, float ViewportMinZ, float ViewportMaxZ, 
												 MatrixParam Projection, MatrixParam View, MatrixParam World) 
		{
			return (Float3*)DirectX::XMVector3UnprojectStream(pOutputStream,OutputStride,pInputStream,InputStride,VectorCount,ViewportX,ViewportY,ViewportWidth,
				ViewportHeight,ViewportMinZ,ViewportMaxZ,Projection,View,World);
		}


		//////////////////////////////////////////////////////////////////////////
		//4D vector operations
		inline static bool     Vector4Equal(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector4Equal(V1,V2);}
		inline static uint32_t Vector4EqualR(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector4EqualR(V1,V2);}
		inline static bool     Vector4EqualInt(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector4EqualInt(V1,V2);}
		inline static uint32_t Vector4EqualIntR(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector4EqualIntR(V1,V2);}
		inline static bool     Vector4NearEqual(Vector4Param V1, Vector4Param V2, Vector4Param Epsilon) {return DirectX::XMVector4NearEqual(V1,V2,Epsilon);}
		inline static bool     Vector4NotEqual(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector4NotEqual(V1,V2);}
		inline static bool     Vector4NotEqualInt(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector4NotEqualInt(V1,V2);}
		inline static bool     Vector4Greater(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector4Greater(V1,V2);}
		inline static uint32_t Vector4GreaterR(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector4GreaterR(V1,V2);}
		inline static bool     Vector4GreaterOrEqual(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector4GreaterOrEqual(V1,V2);}
		inline static uint32_t Vector4GreaterOrEqualR(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector4GreaterOrEqualR(V1,V2);}
		inline static bool     Vector4Less(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector4Less(V1,V2);}
		inline static bool     Vector4LessOrEqual(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector4LessOrEqual(V1,V2);}
		inline static bool     Vector4InBounds(Vector4Param V, Vector4Param Bounds) {return DirectX::XMVector4InBounds(V,Bounds);}
		inline static bool	 Vector4IsNaN(Vector4Param V) {return DirectX::XMVector4IsNaN(V);}
		inline static bool	 Vector4IsInfinite(Vector4Param V) {return DirectX::XMVector4IsInfinite(V);}

		inline static Vector4 XMVector4Dot(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector4Dot(V1,V2);}
		inline static Vector4 XMVector4Cross(Vector4Param V1, Vector4Param V2, Vector4Param V3) {return DirectX::XMVector4Cross(V1,V2,V3);}
		inline static Vector4 XMVector4LengthSq(Vector4Param V) {return DirectX::XMVector4LengthSq(V);}
		inline static Vector4 XMVector4ReciprocalLengthEst(Vector4Param V) {return DirectX::XMVector4ReciprocalLengthEst(V);}
		inline static Vector4 XMVector4ReciprocalLength(Vector4Param V) {return DirectX::XMVector4ReciprocalLength(V);}
		inline static Vector4 XMVector4LengthEst(Vector4Param V) {return DirectX::XMVector4LengthEst(V);}
		inline static Vector4 XMVector4Length(Vector4Param V) {return DirectX::XMVector4Length(V);}
		inline static Vector4 XMVector4NormalizeEst(Vector4Param V) {return DirectX::XMVector4NormalizeEst(V);}
		inline static Vector4 XMVector4Normalize(Vector4Param V) {return DirectX::XMVector4Normalize(V);}
		inline static Vector4 XMVector4ClampLength(Vector4Param V, float LengthMin, float LengthMax) {return DirectX::XMVector4ClampLength(V,LengthMin,LengthMax);}
		inline static Vector4 XMVector4ClampLengthV(Vector4Param V, Vector4Param LengthMin, Vector4Param LengthMax) {return DirectX::XMVector4ClampLengthV(V,LengthMin,LengthMax);}
		inline static Vector4 XMVector4Reflect(Vector4Param Incident, Vector4Param Normal) {return DirectX::XMVector4Reflect(Incident,Normal);}
		inline static Vector4 XMVector4Refract(Vector4Param Incident, Vector4Param Normal, float RefractionIndex) {return DirectX::XMVector4Refract(Incident,Normal,RefractionIndex);}
		inline static Vector4 XMVector4RefractV(Vector4Param Incident, Vector4Param Normal, Vector4Param RefractionIndex) {return DirectX::XMVector4RefractV(Incident,Normal,RefractionIndex);}
		inline static Vector4 XMVector4Orthogonal(Vector4Param V) {return DirectX::XMVector4Orthogonal(V);}
		inline static Vector4 XMVector4AngleBetweenNormalsEst(Vector4Param N1, Vector4Param N2) {return DirectX::XMVector4AngleBetweenNormalsEst(N1,N2);}
		inline static Vector4 XMVector4AngleBetweenNormals(Vector4Param N1, Vector4Param N2) {return DirectX::XMVector4AngleBetweenNormals(N1,N2);}
		inline static Vector4 XMVector4AngleBetweenVectors(Vector4Param V1, Vector4Param V2) {return DirectX::XMVector4AngleBetweenVectors(V1,V2);}
		inline static Vector4 XMVector4Transform(Vector4Param V, MatrixParam M) {return DirectX::XMVector4Transform(V,M);}
		inline static Float4* XMVector4TransformStream(Float4* pOutputStream,size_t OutputStride,const Float4* pInputStream,size_t InputStride, size_t VectorCount, MatrixParam M) 
		{
			return (Float4*)DirectX::XMVector4TransformStream(pOutputStream,OutputStride,pInputStream,InputStride,VectorCount,M);
		}

		//////////////////////////////////////////////////////////////////////////
		//Matrix operations
		inline static bool MatrixIsNaN(MatrixParam M) {return DirectX::XMMatrixIsNaN(M);}
		inline static bool MatrixIsInfinite(MatrixParam M) {return DirectX::XMMatrixIsInfinite(M);}
		inline static bool MatrixIsIdentity(MatrixParam M) {return DirectX::XMMatrixIsIdentity(M);}

		inline static Matrix4x4 MatrixMultiply(MatrixParam M1, MatrixParam M2) {return DirectX::XMMatrixMultiply(M1,M2);}
		inline static Matrix4x4 MatrixMultiplyTranspose(MatrixParam M1, MatrixParam M2) {return DirectX::XMMatrixMultiplyTranspose(M1,M2);}
		inline static Matrix4x4 MatrixTranspose(MatrixParam M) {return DirectX::XMMatrixTranspose(M);}
		inline static Matrix4x4 MatrixInverse(Vector4* pDeterminant, MatrixParam M) {return DirectX::XMMatrixInverse(pDeterminant,M);}
		inline static Vector4   MatrixDeterminant(MatrixParam M) {return DirectX::XMMatrixDeterminant(M);}
		inline static bool      MatrixDecompose(Vector4 *outScale, Vector4 *outRotQuat, Vector4 *outTrans, MatrixParam M) 
		{
			return DirectX::XMMatrixDecompose(outScale,outRotQuat,outTrans,M);
		}

		inline static Matrix4x4 MatrixIdentity() {return DirectX::XMMatrixIdentity();}
		inline static Matrix4x4 MatrixSet(float m00, float m01, float m02, float m03,
				 		float m10, float m11, float m12, float m13,
				 		float m20, float m21, float m22, float m23,
				 		float m30, float m31, float m32, float m33) 
		{
			return DirectX::XMMatrixSet(
				m00, m01, m02,m03,
				m10, m11, m12,m13,
				m20, m21, m22,m23,
				m30, m31, m32,m33);
		}
		inline static Matrix4x4 MatrixTranslation(float OffsetX, float OffsetY, float OffsetZ) {return DirectX::XMMatrixTranslation(OffsetX,OffsetY,OffsetZ);}
		inline static Matrix4x4 MatrixTranslationFromVector(Vector4Param Offset) {return DirectX::XMMatrixTranslationFromVector(Offset);}
		inline static Matrix4x4 MatrixScaling(float ScaleX, float ScaleY, float ScaleZ) {return DirectX::XMMatrixScaling(ScaleX,ScaleY,ScaleZ);}
		inline static Matrix4x4 MatrixScalingFromVector(Vector4Param Scale) {return DirectX::XMMatrixScalingFromVector(Scale);}
		inline static Matrix4x4 MatrixRotationX(float Angle) {return DirectX::XMMatrixRotationX(Angle);}
		inline static Matrix4x4 MatrixRotationY(float Angle) {return DirectX::XMMatrixRotationY(Angle);}
		inline static Matrix4x4 MatrixRotationZ(float Angle) {return DirectX::XMMatrixRotationZ(Angle);}
		inline static Matrix4x4 MatrixRotationRollPitchYaw(float Pitch, float Yaw, float Roll) {return DirectX::XMMatrixRotationRollPitchYaw(Pitch,Yaw,Roll);}
		inline static Matrix4x4 MatrixRotationRollPitchYawFromVector(Vector4Param Angles) {return DirectX::XMMatrixRotationRollPitchYawFromVector(Angles);}
		inline static Matrix4x4 MatrixRotationNormal(Vector4Param NormalAxis, float Angle) {return DirectX::XMMatrixRotationNormal(NormalAxis,Angle);}
		inline static Matrix4x4 MatrixRotationAxis(Vector4Param Axis, float Angle) {return DirectX::XMMatrixRotationAxis(Axis,Angle);}
		inline static Matrix4x4 MatrixRotationQuaternion(Vector4Param Quaternion) {return DirectX::XMMatrixRotationQuaternion(Quaternion);}
		inline static Matrix4x4 MatrixTransformation2D(Vector4Param ScalingOrigin, float ScalingOrientation, Vector4Param Scaling, 
				 					 Vector4Param RotationOrigin, float Rotation, Vector4Param4 Translation) 
		{
			return DirectX::XMMatrixTransformation2D(ScalingOrigin,ScalingOrientation,Scaling,RotationOrigin,Rotation,Translation);
		}
		inline static Matrix4x4 MatrixTransformation(Vector4Param ScalingOrigin, Vector4Param ScalingOrientationQuaternion, Vector4Param Scaling, 
				 				   Vector4Param4 RotationOrigin, Vector4ParamN RotationQuaternion, Vector4ParamN Translation) 
		{
			return DirectX::XMMatrixTransformation(ScalingOrigin,ScalingOrientationQuaternion,Scaling,RotationOrigin,RotationQuaternion,Translation);
		}
		inline static Matrix4x4 MatrixAffineTransformation2D(Vector4Param Scaling, Vector4Param RotationOrigin, float Rotation, Vector4Param Translation) 
		{
			return DirectX::XMMatrixAffineTransformation2D(Scaling,RotationOrigin,Rotation,Translation);
		}
		inline static Matrix4x4 MatrixAffineTransformation(Vector4Param Scaling, Vector4Param RotationOrigin, Vector4Param RotationQuaternion, Vector4Param4 Translation) 
		{
			return DirectX::XMMatrixAffineTransformation(Scaling,RotationOrigin,RotationQuaternion,Translation);
		}
		inline static Matrix4x4 MatrixReflect(Vector4Param ReflectionPlane) {return DirectX::XMMatrixReflect(ReflectionPlane);}
		inline static Matrix4x4 MatrixShadow(Vector4Param ShadowPlane, Vector4Param LightPosition) {return DirectX::XMMatrixShadow(ShadowPlane,LightPosition);}
				  
		inline static Matrix4x4 MatrixLookAtLH(Vector4Param EyePosition, Vector4Param FocusPosition, Vector4Param UpDirection) {return DirectX::XMMatrixLookAtLH(EyePosition,FocusPosition,UpDirection);}
		inline static Matrix4x4 MatrixLookAtRH(Vector4Param EyePosition, Vector4Param FocusPosition, Vector4Param UpDirection) {return DirectX::XMMatrixLookAtRH(EyePosition,FocusPosition,UpDirection);}
		inline static Matrix4x4 MatrixLookToLH(Vector4Param EyePosition, Vector4Param EyeDirection, Vector4Param UpDirection)  {return DirectX::XMMatrixLookToLH(EyePosition,EyeDirection,UpDirection);}
		inline static Matrix4x4 MatrixLookToRH(Vector4Param EyePosition, Vector4Param EyeDirection, Vector4Param UpDirection)  {return DirectX::XMMatrixLookToRH(EyePosition,EyeDirection,UpDirection);}
		inline static Matrix4x4 MatrixPerspectiveLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ) {return DirectX::XMMatrixPerspectiveLH(ViewWidth,ViewHeight,NearZ,FarZ);}
		inline static Matrix4x4 MatrixPerspectiveRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ) {return DirectX::XMMatrixPerspectiveRH(ViewWidth,ViewHeight,NearZ,FarZ);}
		inline static Matrix4x4 MatrixPerspectiveFovLH(float FovAngleY, float AspectHByW, float NearZ, float FarZ) {return DirectX::XMMatrixPerspectiveFovLH(FovAngleY,AspectHByW,NearZ,FarZ);}
		inline static Matrix4x4 MatrixPerspectiveFovRH(float FovAngleY, float AspectHByW, float NearZ, float FarZ) {return DirectX::XMMatrixPerspectiveFovRH(FovAngleY,AspectHByW,NearZ,FarZ);}
		inline static Matrix4x4 MatrixPerspectiveOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ) 
		{
			return DirectX::XMMatrixPerspectiveOffCenterLH(ViewLeft,ViewRight,ViewBottom,ViewTop,NearZ,FarZ);
		}
		inline static Matrix4x4 MatrixPerspectiveOffCenterRH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ) 
		{
			return DirectX::XMMatrixPerspectiveOffCenterRH(ViewLeft,ViewRight,ViewBottom,ViewTop,NearZ,FarZ);
		}
		inline static Matrix4x4 MatrixOrthographicLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ) 
		{
			return DirectX::XMMatrixOrthographicLH(ViewWidth,ViewHeight,NearZ,FarZ);
		}
		inline static Matrix4x4 MatrixOrthographicRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ) 
		{
			return DirectX::XMMatrixOrthographicRH(ViewWidth,ViewHeight,NearZ,FarZ);
		}
		inline static Matrix4x4 MatrixOrthographicOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ) 
		{
			return DirectX::XMMatrixOrthographicOffCenterLH(ViewLeft,ViewRight,ViewBottom,ViewTop,NearZ,FarZ);
		}
		inline static Matrix4x4 MatrixOrthographicOffCenterRH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ) 
		{
			return DirectX::XMMatrixOrthographicOffCenterRH(ViewLeft,ViewRight,ViewBottom,ViewTop,NearZ,FarZ);
		}

		//////////////////////////////////////////////////////////////////////////
		// Quaternion operations
		inline static bool QuaternionEqual(Vector4Param Q1, Vector4Param Q2) {return DirectX::XMQuaternionEqual(Q1,Q2);}
		inline static bool QuaternionNotEqual(Vector4Param Q1, Vector4Param Q2) {return DirectX::XMQuaternionNotEqual(Q1,Q2);}
			 
		inline static bool QuaternionIsNaN(Vector4Param Q) {return DirectX::XMQuaternionIsNaN(Q);}
		inline static bool QuaternionIsInfinite(Vector4Param Q) {return DirectX::XMQuaternionIsInfinite(Q);}
		inline static bool QuaternionIsIdentity(Vector4Param Q) {return DirectX::XMQuaternionIsIdentity(Q);}

		inline static Vector4 QuaternionDot(Vector4Param Q1, Vector4Param Q2) {return DirectX::XMQuaternionDot(Q1,Q2);}
		inline static Vector4 QuaternionMultiply(Vector4Param Q1, Vector4Param Q2) {return DirectX::XMQuaternionMultiply(Q1,Q2);}
		inline static Vector4 QuaternionLengthSq(Vector4Param Q) {return DirectX::XMQuaternionLengthSq(Q);}
		inline static Vector4 QuaternionReciprocalLength(Vector4Param Q) {return DirectX::XMQuaternionReciprocalLength(Q);}
		inline static Vector4 QuaternionLength(Vector4Param Q) {return DirectX::XMQuaternionLength(Q);}
		inline static Vector4 QuaternionNormalizeEst(Vector4Param Q) {return DirectX::XMQuaternionNormalizeEst(Q);}
		inline static Vector4 QuaternionNormalize(Vector4Param Q) {return DirectX::XMQuaternionNormalize(Q);}
		inline static Vector4 QuaternionConjugate(Vector4Param Q) {return DirectX::XMQuaternionConjugate(Q);}
		inline static Vector4 QuaternionInverse(Vector4Param Q) {return DirectX::XMQuaternionInverse(Q);}
		inline static Vector4 QuaternionLn(Vector4Param Q) {return DirectX::XMQuaternionLn(Q);}
		inline static Vector4 QuaternionExp(Vector4Param Q) {return DirectX::XMQuaternionExp(Q);}
		inline static Vector4 QuaternionSlerp(Vector4Param Q0, Vector4Param Q1, float t) {return DirectX::XMQuaternionSlerp(Q0,Q1,t);}
		inline static Vector4 QuaternionSlerpV(Vector4Param Q0, Vector4Param Q1, Vector4Param T) {return DirectX::XMQuaternionSlerpV(Q0,Q1,T);}
		inline static Vector4 QuaternionSquad(Vector4Param Q0, Vector4Param Q1, Vector4Param Q2, Vector4Param4 Q3, float t) 
		{
			return DirectX::XMQuaternionSquad(Q0,Q1,Q2,Q3,t);
		}
		inline static Vector4 QuaternionSquadV(Vector4Param Q0, Vector4Param Q1, Vector4Param Q2, Vector4Param4 Q3, Vector4ParamN T) 
		{
			return DirectX::XMQuaternionSquadV(Q0,Q1,Q2,Q3,T);
		}
		inline static void    QuaternionSquadSetup(Vector4* pA, Vector4* pB, Vector4* pC, Vector4Param Q0, Vector4Param Q1, Vector4Param Q2, Vector4Param4 Q3) 
		{
			return DirectX::XMQuaternionSquadSetup(pA,pB,pC,Q0,Q1,Q2,Q3);
		}
		inline static Vector4 QuaternionBaryCentric(Vector4Param Q0, Vector4Param Q1, Vector4Param Q2, float f, float g) 
		{
			return DirectX::XMQuaternionBaryCentric(Q0,Q1,Q2,f,g);
		}
		inline static Vector4 QuaternionBaryCentricV(Vector4Param Q0, Vector4Param Q1, Vector4Param Q2, Vector4Param4 F, Vector4ParamN G) 
		{
			return DirectX::XMQuaternionBaryCentricV(Q0,Q1,Q2,F,G);
		}
			    
		inline static Vector4 QuaternionIdentity() {return DirectX::XMQuaternionIdentity();}
		inline static Vector4 QuaternionRotationRollPitchYaw(float Pitch, float Yaw, float Roll) {return DirectX::XMQuaternionRotationRollPitchYaw(Pitch,Yaw,Roll);}
		inline static Vector4 QuaternionRotationRollPitchYawFromVector(Vector4Param Angles) {return DirectX::XMQuaternionRotationRollPitchYawFromVector(Angles);}
		inline static Vector4 QuaternionRotationNormal(Vector4Param NormalAxis, float Angle) {return DirectX::XMQuaternionRotationNormal(NormalAxis,Angle);}
		inline static Vector4 QuaternionRotationAxis(Vector4Param Axis, float Angle) {return DirectX::XMQuaternionRotationAxis(Axis,Angle);}
		inline static Vector4 QuaternionRotationMatrix(MatrixParam M) {return DirectX::XMQuaternionRotationMatrix(M);}

		inline static void    QuaternionToAxisAngle(Vector4* pAxis, float* pAngle, Vector4Param Q) {return DirectX::XMQuaternionToAxisAngle(pAxis,pAngle,Q);}


		//////////////////////////////////////////////////////////////////////////
		//Plane operations
		inline static bool PlaneEqual(Vector4Param P1, Vector4Param P2) {return DirectX::XMPlaneEqual(P1,P2);}
		inline static bool PlaneNearEqual(Vector4Param P1, Vector4Param P2, Vector4Param Epsilon) {return DirectX::XMPlaneNearEqual(P1,P2,Epsilon);}
		inline static bool PlaneNotEqual(Vector4Param P1, Vector4Param P2) {return DirectX::XMPlaneNotEqual(P1,P2);}

		inline static bool PlaneIsNaN(Vector4Param P) {return DirectX::XMPlaneIsNaN(P);}
		inline static bool PlaneIsInfinite(Vector4Param P) {return DirectX::XMPlaneIsInfinite(P);}

		inline static Vector4 PlaneDot(Vector4Param P, Vector4Param V) {return DirectX::XMPlaneDot(P,V);}
		inline static Vector4 PlaneDotCoord(Vector4Param P, Vector4Param V) {return DirectX::XMPlaneDotCoord(P,V);}
		inline static Vector4 PlaneDotNormal(Vector4Param P, Vector4Param V) {return DirectX::XMPlaneDotNormal(P,V);}
		inline static Vector4 PlaneNormalizeEst(Vector4Param P) {return DirectX::XMPlaneNormalizeEst(P);}
		inline static Vector4 PlaneNormalize(Vector4Param P) {return DirectX::XMPlaneNormalize(P);}
		inline static Vector4 PlaneIntersectLine(Vector4Param P, Vector4Param LinePoint1, Vector4Param LinePoint2) {return DirectX::XMPlaneIntersectLine(P,LinePoint1,LinePoint2);}
		inline static void    PlaneIntersectPlane(Vector4* pLinePoint1, Vector4* pLinePoint2, Vector4Param P1, Vector4Param P2) {return DirectX::XMPlaneIntersectPlane(pLinePoint1,pLinePoint2,P1,P2);}
		inline static Vector4 PlaneTransform(Vector4Param P, MatrixParam M) {return DirectX::XMPlaneTransform(P,M);}
		inline static Float4* PlaneTransformStream(Float4* pOutputStream,size_t OutputStride,const Float4* pInputStream,size_t InputStride, size_t PlaneCount, MatrixParam M) 
		{
			return (Float4*)DirectX::XMPlaneTransformStream(pOutputStream,OutputStride,pInputStream,InputStride,PlaneCount,M);
		}
		inline static Vector4 PlaneFromPointNormal(Vector4Param Point, Vector4Param Normal) {return DirectX::XMPlaneFromPointNormal(Point,Normal);}
		inline static Vector4 PlaneFromPoints(Vector4Param Point1, Vector4Param Point2, Vector4Param Point3) {return DirectX::XMPlaneFromPoints(Point1,Point2,Point3);}

		//////////////////////////////////////////////////////////////////////////
		//Color operations
		inline static bool ColorEqual(Vector4Param C1, Vector4Param C2) {return DirectX::XMColorEqual(C1,C2);}
		inline static bool ColorNotEqual(Vector4Param C1, Vector4Param C2) {return DirectX::XMColorNotEqual(C1,C2);}
		inline static bool ColorGreater(Vector4Param C1, Vector4Param C2) {return DirectX::XMColorGreater(C1,C2);}
		inline static bool ColorGreaterOrEqual(Vector4Param C1, Vector4Param C2) {return DirectX::XMColorGreaterOrEqual(C1,C2);}
		inline static bool ColorLess(Vector4Param C1, Vector4Param C2) {return DirectX::XMColorLess(C1,C2);}
		inline static bool ColorLessOrEqual(Vector4Param C1, Vector4Param C2) {return DirectX::XMColorLessOrEqual(C1,C2);}
			 
		inline static bool ColorIsNaN(Vector4Param C) {return DirectX::XMColorIsNaN(C);}
		inline static bool ColorIsInfinite(Vector4Param C) {return DirectX::XMColorIsInfinite(C);}

		inline static Vector4 ColorNegative(Vector4Param C) {return DirectX::XMColorNegative(C);}
		inline static Vector4 ColorModulate(Vector4Param C1, Vector4Param C2) {return DirectX::XMColorModulate(C1,C2);}
		inline static Vector4 ColorAdjustSaturation(Vector4Param C, float Saturation) {return DirectX::XMColorAdjustSaturation(C, Saturation);}
		inline static Vector4 ColorAdjustContrast(Vector4Param C, float Contrast) {return DirectX::XMColorAdjustContrast(C,Contrast);}

		inline static Vector4 ColorRGBToHSL( Vector4Param rgb ) {return DirectX::XMColorRGBToHSL(rgb);}
		inline static Vector4 ColorHSLToRGB( Vector4Param hsl ) {return DirectX::XMColorHSLToRGB(hsl);}

		inline static Vector4 ColorRGBToHSV( Vector4Param rgb ) {return DirectX::XMColorRGBToHSV(rgb);}
		inline static Vector4 ColorHSVToRGB( Vector4Param hsv ) {return DirectX::XMColorHSVToRGB(hsv);}

		inline static Vector4 ColorRGBToYUV( Vector4Param rgb ) {return DirectX::XMColorRGBToYUV(rgb);}
		inline static Vector4 ColorYUVToRGB( Vector4Param yuv ) {return DirectX::XMColorYUVToRGB(yuv);}

		inline static Vector4 ColorRGBToYUV_HD( Vector4Param rgb ) {return DirectX::XMColorRGBToYUV_HD(rgb);}
		inline static Vector4 ColorYUVToRGB_HD( Vector4Param yuv ) {return DirectX::XMColorYUVToRGB_HD(yuv);}

		inline static Vector4 ColorRGBToXYZ( Vector4Param rgb ) {return DirectX::XMColorRGBToXYZ(rgb);}
		inline static Vector4 ColorXYZToRGB( Vector4Param xyz ) {return DirectX::XMColorXYZToRGB(xyz);}

		inline static Vector4 ColorXYZToSRGB( Vector4Param xyz ) {return DirectX::XMColorXYZToSRGB(xyz);}
		inline static Vector4 ColorSRGBToXYZ( Vector4Param srgb) {return DirectX::XMColorSRGBToXYZ(srgb);}

		inline static Vector4 FresnelTerm(Vector4Param CosIncidentAngle, Vector4Param RefractionIndex)
		{
			return DirectX::XMFresnelTerm(CosIncidentAngle,RefractionIndex);
		}
	};
#include "FlameMath.inl"
}