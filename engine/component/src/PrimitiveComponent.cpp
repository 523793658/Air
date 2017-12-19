#include "Context.h"
#include "ContentStream.hpp"
#include "component/include/PrimitiveComponent.hpp"

namespace Air
{
	void PrimitiveComponent::setMaterial(int32_t elementIndex, class MaterialInterface* material)
	{

	}

	MaterialInstanceDynamic* PrimitiveComponent::createAndSetMaterialInstanceDynamic(int32_t elementIndex)
	{
		return nullptr;
	}

	MaterialInstanceDynamic* PrimitiveComponent::createAndSetMaterialInstanceDynamicFromMaterial(int32_t elementIndex, class MaterialInterface* parent)
	{
		return nullptr;
	}

	MaterialInstanceDynamic* PrimitiveComponent::createDynamicMaterialInstance(int32_t elementIndex, class MaterialInterface* sourceMaterial /* = nullptr */)
	{
		return nullptr;
	}

	float4x4 PrimitiveComponent::getRenderMatrix() const
	{
		return float4x4::identify();
	}

	int32_t PrimitiveComponent::getNumMaterials() const
	{
		return 0;
	}

	void PrimitiveComponent::onRegister()
	{
		ComponentBase::onRegister();
		IStreamingManager::get().notifyPrimitiveUpdated(this);
	}




}