#include "Engine.h"
#include "Context.h"
#include "rendersystem/include/RenderFactory.h"
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


	void Renderable::addInstance(SceneObject const * obj)
	{

	}

	void Renderable::addToRenderQueue()
	{

	}

	void Renderable::render()
	{
		this->updateInstanceStream();
		RenderEngine& re = Context::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
		RenderLayout const & layout = this->getRenderLayout();

	}

}