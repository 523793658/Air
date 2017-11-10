#ifndef _SceneInterface_H_
#define _SceneInterface_H_
#pragma once
#include "PreDeclare.h"
namespace Air
{
	class SceneInterface
	{
		virtual void addPrimitive(PrimitiveComponent* primitive) = 0;

		virtual void removePrimitive(PrimitiveComponent* primitive) = 0;

		virtual void releasePrimitive(PrimitiveComponent* primitive) = 0;

		virtual void updatePrimitiveTransform(PrimitiveComponent* primitive) = 0;

		virtual void updatePrimitiveAttachment(PrimitiveComponent* primitive) = 0;

		virtual void getPrimitiveSceneInfo(int32_t primitiveIndex) = 0;	

		virtual void addLight(LightComponent* light) = 0;

		virtual void removeLight(LightComponent* light) = 0;

		virtual void addInvisibleLight(LightComponent * light) = 0;

		virtual void setSkyLight(SkyLightSceneProxy* light) = 0;

		virtual void disableSkyLight(SkyLightSceneProxy* light) = 0;

		virtual void addDecal(DecalComponent* component) = 0;

		virtual void removeDecal(DecalComponent* component) = 0;

		virtual void updateDecalTransform(DecalComponent* component) = 0;

		virtual void addReflectionCapture(class ReflectionCaptureComponent* component) {}

		virtual void removeReflectionCapture(class ReflectionCaptureComponent* component) {}

		virtual void getReflectionCaptureData(ReflectionCaptureComponent* component, class ReflectionCaptureFullHDR& outDerivedData) {}

		virtual void updateReflectionCaptureTransform(class ReflectionCaptureComponent* component) {}

		virtual void allocateRefelctionCaptures(const std::vector<ReflectionCaptureComponent*>& newCaptures) {}

		virtual void releaseReflectionCubemap(ReflectionCaptureComponent* captureComponent) {}

		virtual void updateSkyCaptureContents(const SkyLightComponent* captureComponent, bool bCaptureEmissiveOnly, Texture* sourceCubemap, Texture* outProcessedTexture, float & outaverageBrightness, std::vector<float3>& OutIrradianceEnvironmentMap) {}


		virtual void precullStaticMeshes(const std::vector<StaticMeshComponent*>& componentsToPreCull, const std::vector<std::vector<Plane>>& cullValumes) {}

		virtual void updateLightTransform(LightComponent* light) = 0;

		virtual void updateLightColorAndBrightness(LightComponent* light) = 0;

		virtual void setProcomputedVisiblity(const class PrecomputedVisibilityHandler* precomputedVisibilityHandler) {}

		virtual void release() = 0;

		virtual void getRelevantLights(PrimitiveComponent* primitive, std::vector<const LightComponent*>* relevantLights) const = 0;

		virtual bool requiresHitProxies() const = 0;

		virtual class World* getWorld() const = 0;

		virtual class Scene* getRenderScene()
		{
			return nullptr;
		}

		virtual void updateSceneSetting(WorldSettings* worldSettings) {}


	};




}



#endif
