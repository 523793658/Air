#include "FlameMath.h"
#include "DirectXMathConvert.inl"
#include "DirectXMathVector.inl"
#include "DirectXMathMatrix.inl"
#include "DirectXMathMisc.inl"
namespace Air
{
#include "FlameMath.inl"

	Vector4 operator+ (Vector4Param v) { return v; }
	Vector4 operator- (Vector4Param v) { return DirectX::operator -(v); }

	Vector4& operator+= (Vector4& V1, Vector4Param V2) { return DirectX::operator +=(V1, V2); }
	Vector4& operator-= (Vector4& V1, Vector4Param V2) { return DirectX::operator -= (V1, V2); }
	Vector4& operator*= (Vector4& V1, Vector4Param V2) { return DirectX::operator *= (V1, V2); }
	Vector4& operator/= (Vector4& V1, Vector4Param V2) { return DirectX::operator /= (V1, V2); }
	Vector4& operator*= (Vector4& V, float S) { return DirectX::operator *= (V, S); }
	Vector4& operator/= (Vector4& V, float S) { return DirectX::operator /= (V, S); }

	Vector4 operator+ (Vector4Param V1, Vector4Param V2) { return DirectX::operator +(V1, V2); }
	Vector4 operator- (Vector4Param V1, Vector4Param V2) { return DirectX::operator -(V1, V2); }
	Vector4 operator* (Vector4Param V1, Vector4Param V2) { return DirectX::operator *(V1, V2); }
	Vector4 operator/ (Vector4Param V1, Vector4Param V2) { return DirectX::operator /(V1, V2); }
	Vector4 operator* (Vector4Param V, float S) { return DirectX::operator *(V, S); }
	Vector4 operator* (float S, Vector4Param V) { return DirectX::operator *(S, V); }
	Vector4 operator/ (Vector4Param V, float S) { return DirectX::operator /(V, S); }
}