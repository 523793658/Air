#include "Engine.h"
#include "Context.h"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/FrameBuffer.hpp"
#include "rendersystem/include/RenderEngine.hpp"
#include "rendersystem/include/Viewport.hpp"
#include "Camera.hpp"
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

	void Renderable::onRenderBegin()
	{
// 		RenderEngine& re = Context::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
// 		Camera const & camera = *re.getCurrentFrameBuffer()->getViewport()->mCamera;
// 		float4x4 const & vp = camera.getViewProjMatrix();
// 		float4x4 mvp = mModelMat * vp;
// 		AABBox const & pos_bb = this->getPosAABB();
// 		AABBox const & tc_bb = this->getTexcoordAABB();
	}

	void Renderable::onRenderEnd()
	{

	}

	void Renderable::onInstanceBegin(uint32_t id)
	{

	}
	void Renderable::onInstanceEnd(uint32_t id)
	{

	}

	void Renderable::setLocalMatrix(float4x4 const & mat)
	{
		mModelMat = mat;
	}
	
	void Renderable::setObjectID(uint32_t id)
	{
		mSelectModeObjectId = float4(((id & 0xff) + 0.5f) / 255.0f, (((id >> 8) & 0xff) + 0.5f) / 255.0f, (((id >> 16) & 0xff) + 0.5f) / 255.0f, 0.0);
	}

	void Renderable::updateAABB()
	{

	}

	void Renderable::updateInstanceStream()
	{

	}

}