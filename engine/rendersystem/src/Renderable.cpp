#include "Engine.h"

#include "rendersystem/include/Renderable.hpp"

namespace Air
{
	Renderable::Renderable()
	{

	}
	Renderable::~Renderable()
	{
	}

	bool Renderable::getHWResourceReady() const
	{
		return true;
	}

}