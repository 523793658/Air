#ifndef _GUID_H_
#define _GUID_H_
#pragma once

#include "HAL/CoreTypes.h"
#include "basic/include/ResIdentifier.h"
#include "boost/lexical_cast.hpp"

namespace Air
{


enum class EGuidFormats
{
	/**
	* 32 digits.
	*
	* For example: "00000000000000000000000000000000"
	*/
	Digits,

	/**
	* 32 digits separated by hyphens.
	*
	* For example: 00000000-0000-0000-0000-000000000000
	*/
	DigitsWithHyphens,

	/**
	* 32 digits separated by hyphens and enclosed in braces.
	*
	* For example: {00000000-0000-0000-0000-000000000000}
	*/
	DigitsWithHyphensInBraces,

	/**
	* 32 digits separated by hyphens and enclosed in parentheses.
	*
	* For example: (00000000-0000-0000-0000-000000000000)
	*/
	DigitsWithHyphensInParentheses,

	/**
	* Comma-separated hexadecimal values enclosed in braces.
	*
	* For example: {0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}}
	*/
	HexValuesInBraces,

	/**
	* This format is currently used by the FUniqueObjectGuid class.
	*
	* For example: 00000000-00000000-00000000-00000000
	*/
	UniqueObjectGuid,
};

struct Guid
{
private:
	uint32_t A;
	uint32_t B;
	uint32_t C;
	uint32_t D;

public:
	Guid()
		:A(0),
		B(0),
		C(0),
		D(0)
	{

	}

	Guid(uint32_t a, uint32_t b, uint32_t c, uint32_t d)
		:A(a), B(b), C(c), D(d)
	{

	}

public:
	friend bool operator == (const Guid & x, const Guid& y)
	{
		return ((x.A ^ y.A) | (x.B ^ y.B) | (x.C ^ y.C) | (x.D ^ y.D)) == 0;
	}

	friend bool operator != (const Guid & x, const Guid & y)
	{
		return ((x.A ^ y.A) | (x.B ^ y.B) | (x.C ^ y.C) | (x.D ^ y.D)) != 0;
	}

	friend bool operator < (const Guid & X, const Guid & Y)
	{
		return	((X.A < Y.A) ? true : ((X.A > Y.A) ? false :
			((X.B < Y.B) ? true : ((X.B > Y.B) ? false :
			((X.C < Y.C) ? true : ((X.C > Y.C) ? false :
				((X.D < Y.D) ? true : ((X.D > Y.D) ? false : false)))))))); //-V583
	}

	uint32_t& operator[](int32_t index)
	{
		switch (index)
		{
		case 0:
			return A;
		case 1:
			return B;
		case 2:
			return C;
		case 3:
			return D;
		}
		return A;
	}

	const uint32_t& operator[](uint32_t Index) const
	{

		switch (Index)
		{
		case 0: return A;
		case 1: return B;
		case 2: return C;
		case 3: return D;
		}

		return A;
	}

	friend Air::Archive& operator << (Air::Archive & archive, Guid& G)
	{
		return archive << G.A << G.B << G.C << G.D;
	}

	bool serialize(Air::Archive& ar)
	{
		ar << *this;
		return true;
	}

	void invalidate()
	{
		A = B = C = D = 0;
	}

	bool isValid() const
	{
		return ((A | B | C | D) != 0);
	}

	std::string toString() const
	{
		return boost::lexical_cast<std::string>(static_cast<int32_t>(EGuidFormats::Digits));
	}

	std::wstring toWString() const
	{
		return boost::lexical_cast<std::wstring>(static_cast<int32_t>(EGuidFormats::Digits));
	}

public:
	friend uint32_t getTypeHash(const Guid & guid)
	{
		return boost::hash_range(&guid.A, (&guid.A) + 4);
	}

public:
	static Guid newGuid();

	static bool parse(const std::string guidString, Guid & guid);

	static bool parseExact(const std::string& GuidString, EGuidFormats Format, Guid& OutGuid);
};

}


#endif
