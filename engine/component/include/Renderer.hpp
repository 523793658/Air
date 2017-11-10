#ifndef _Renderer_H_
#define _Renderer_H_
#pragma once
#include "ComponentBase.hpp"
namespace Air
{
	class AIR_CORE_API PrimitiveComponent : public ComponentBase
	{
		virtual void setMaterial(int32_t elementIndex, class MaterialInterface* material);

		virtual class MaterialInstanceDynamic* createAndSetMaterialInstanceDynamic(int32_t elementIndex);

		virtual class MaterialInstanceDynamic * createAndSetMaterialInstanceDynamicFromMaterial(int32_t elementIndex, class MaterialInterface* parent);

		virtual class MaterialInstanceDynamic* createDynamicMaterialInstance(int32_t elementIndex, class MaterialInterface* sourceMaterial = nullptr);

		void setCastShadow(bool newCastShadow);

		void setTranslucentSortPriority(int32_t newTranslucentSortPriority);

		void setRenderCustomDepth(bool bValue);

		void setCustomDepthStencilValue(int32_t value);

		void setRenderInMainPass(bool bValue);

		void setRenderInMono(bool bValue);

		virtual float4x4 getRenderMatrix() const;

		virtual int32_t getNumMaterials() const;
	public:
		static int32_t mCurrentTag;

	private:
		class PrimitiveComponent* mLODParentPrimitive;

	public:
		void setLODParentPrimitive()

	};




}

#endif
