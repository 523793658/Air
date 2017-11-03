#include "Engine.h"
#include "Context.h"
#include "rendersystem/include/RenderFactory.h"
#include "rendersystem/include/FrameBuffer.hpp"
#include "rendersystem/include/RenderEngine.hpp"
#include "rendersystem/include/Viewport.hpp"
#include "Camera.hpp"
#include "rendersystem/include/RenderLayout.hpp"
#include "scene_manager/include/SceneManager.hpp"
#include "rendersystem/include/RenderEffect.hpp"
#include "rendersystem/include/RenderMaterial.hpp"
#include "rendersystem/include/Renderable.hpp"

namespace Air
{
	Renderable::Renderable()
	{
		RenderEffectPtr effect = syncLoadRenderEffect("assets/shader/simpleforward.asd");

		bindEffect(effect);
		mTechnique = effect->getTechniqueByIndex(0);
	}
	Renderable::~Renderable()
	{
	}

	bool Renderable::getHWResourceReady() const
	{
		return true;
	}

	bool Renderable::isTransparencyBackFace() const
	{
		return mEffectAttrs & EA_TransparencyBack ? true : false;
	}

	bool Renderable::isTransparencyFrontFace() const
	{
		return mEffectAttrs & EA_TransparencyFront ? true : false;
	}


	void Renderable::addInstance(SceneObject const * obj)
	{

	}

	void Renderable::addToRenderQueue()
	{
		Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance().addRenderableToQueue(this);
	}

	void Renderable::render()
	{
		this->updateInstanceStream();
		RenderEngine& re = Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
		RenderLayout const & layout = this->getRenderLayout();
		GraphicsBufferPtr const & inst_stream = layout.getInstanceStream();
		RenderTechnique const & tech = *this->getRenderTechnique();
		auto const & effect = *this->getRenderEffect();
		if (inst_stream)
		{
			if (layout.getNumIndices() > 0)
			{
				this->onRenderBegin();
				re.render(effect, tech, layout);
				this->onRenderEnd();
			}
		}
		else
		{
			this->onRenderBegin();
			if (mInstances.empty())
			{
				re.render(effect, tech, layout);
			}
			else
			{
				for (uint32_t i = 0; i < mInstances.size(); ++i)
				{
					this->onInstanceBegin(i);
					re.render(effect, tech, layout);
					this->onInstanceEnd(i);
				}
			}
			this->onRenderEnd();
		}
	}

	void Renderable::setMaterial(RenderMaterialPtr material)
	{
		mMaterial = material;
	}

	void Renderable::onRenderBegin()
	{
		RenderEngine& re = Engine::getInstance().getRenderFactoryInstance().getRenderEngineInstance();
		Camera const & camera = *re.getCurrentFrameBuffer()->getViewport()->mCamera;
		float4x4 const & vp = camera.getViewProjMatrix();
		float4x4 mvp = mModelMat * vp;
		AABBox const & pos_bb = this->getPosAABB();
		AABBox const & tc_bb = this->getTexcoordAABB();
		*mMVPParam = mvp;
		*mWorldParam = mModelMat;
		if (mMaterial)
		{
			*mSpecularColorMetallic = mMaterial->mSpecularMetalness;
			*mBaseColorRoughness = mMaterial->mAlbedoRoughness;
		}
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

	void Renderable::setMatrix(float4x4 const & mat)
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

	void Renderable::bindEffect(RenderEffectPtr const & effect)
	{
		mEffect = effect;
		this->updateTechniques();

		mMVPParam = effect->getParameterByName("mvp");
		mWorldParam = effect->getParameterByName("worldMatrix");
		mSpecularColorMetallic = effect->getParameterByName("u_SpecularColorMetallic");
		mBaseColorRoughness = effect->getParameterByName("u_BaseColorRoughness");

	}
	RenderTechnique* Renderable::getPassTechnique(PassType type) const
	{
		switch (type)
		{
		case Air::PT_OpaqueDepth:
			break;
		case Air::PT_TransparencyBackDepth:
			break;
		case Air::PT_TransparencyFrontDepth:
			break;
		case Air::PT_OpaqueGBufferRT0:
			break;
		case Air::PT_TransparencyBackGBufferRT0:
			break;
		case Air::PT_TransparencyFrontGBufferRT0:
			break;
		case Air::PT_OpaqueGBufferRT1:
			break;
		case Air::PT_TransparencyBackGBufferRT1:
			break;
		case Air::PT_TransparencyFrontGBufferRT1:
			break;
		case Air::PT_OpaqueGBufferMRT:
			break;
		case Air::PT_TransparencyBackGBufferMRT:
			break;
		case Air::PT_TransparencyFrontGBufferMRT:
			break;
		case Air::PT_GenShadowMap:
			break;
		case Air::PT_GenShadowMapWODepthTexture:
			break;
		case Air::PT_GenCascadedShadowMap:
			break;
		case Air::PT_GenReflectiveShadowMap:
			break;
		case Air::PT_Shadowing:
			break;
		case Air::PT_IndirectLighting:
			break;
		case Air::PT_OpaqueShading:
			break;
		case Air::PT_TransparencyBackShading:
			break;
		case Air::PT_TransparencyFrontShading:
			break;
		case Air::PT_OpaqueReflection:
			break;
		case Air::PT_TransparencyBackReflection:
			break;
		case Air::PT_TransparencyFrontReflection:
			break;
		case Air::PT_OpaqueSpecialShading:
			break;
		case Air::PT_TransparencyBackSpecialShading:
			break;
		case Air::PT_TransparencyFrontSpecialShading:
			break;
		case Air::PT_SimpleForward:
			return mSimpleForwardTech;
			break;
		default:
			break;
		}
		return mSimpleForwardTech;
	}

	void Renderable::updateTechniques()
	{
		RenderMaterial::SurfaceDetailMode sdm;
		if (mMaterial)
		{
			sdm = mMaterial->mDetailMode;
		}
		else
		{
			sdm = RenderMaterial::SDM_Parallax;
		}
		switch (sdm)
		{
		case Air::RenderMaterial::SDM_Parallax:
			break;
		case Air::RenderMaterial::SDM_FlatTessellation:
			break;
		case Air::RenderMaterial::SDM_SmoothTessellation:
			break;
		default:
			break;
		}

		mSimpleForwardTech = mEffect->getTechniqueByName("simpleForward");

	}
}