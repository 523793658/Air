#ifndef _RenderEffect_H_
#define _RenderEffect_H_

#include <boost/noncopyable.hpp>

namespace Air
{
	class AIR_CORE_API RenderTechnique : boost::noncopyable
	{
	public:
		float getWeight() const;
	};
}


#endif