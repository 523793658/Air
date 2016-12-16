#ifndef _AABBBox_H_
#define _AABBBox_H_
#pragma once

namespace Air
{
#ifdef FLAMEMATH
	typedef AABB AABBox;
#else
	class AABBox
	{

	};
#endif
}
#endif