#ifndef _MaterialInterface_H_
#define _MaterialInterface_H_
#pragma once
#include "rendersystem/include/RHIDefinitions.hpp"
namespace Air
{
	enum ETextureIndex
	{
		ETI_None = -1,

	};

	struct MaterialTextureInfo
	{
		MaterialTextureInfo()
		{

		}

		float mSamplingScale{ 0.0f };

		int32_t mUVChannelIndex{ ETI_None };

		std::string mTextureName;

		bool isValid(bool bCheckTextureIndex = false) const;
	};


	class MaterialInterface
	{
		class SubsurfaceProfile* mSubsurfaceProfile;
		std::vector<MaterialTextureInfo> mTextureStreamingData;
	public:


	private:
		uint32_t mFeatureLevelsToForceCompile;
		static uint32_t mFeatureLevelsForAllMaterials;

	public:
		void setFeatureLevelToCompile(ERHIFeatureLevel::Type featureLevel, bool bShouldCompile);

		static void setGlobalRequiredFeatureLevel(ERHIFeatureLevel::Type featureLevel, bool bShouldCompile);


	};
}
#endif
