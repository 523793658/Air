#include "ResourcePluginFBX.h"
#include "basic/include/Log.hpp"
#include "core/include/ResourcePlugin.h"
namespace Air
{

	ResourcePluginFBX::ResourcePluginFBX() : ResourcePlugin("ResourcePluginFBX", "fbx")
	{
		mSDKManager = FbxManager::Create();
		if (!mSDKManager)
		{
			logError("Unable to create FBX Manager");
		}
		FbxIOSettings* ios = FbxIOSettings::Create(mSDKManager, IOSROOT);
		mSDKManager->SetIOSettings(ios);
	}


	void ResourcePluginFBX::importResource(ResLoadingDescPtr& res_desc)
	{
		

	}
	void ResourcePluginFBX::exportResource(ResLoadingDescPtr& res_desc)
	{

	}

}











void makeResourcePlugin(std::unique_ptr<Air::ResourcePlugin>& ptr)
{
	ptr = Air::MakeUniquePtr<Air::ResourcePluginFBX>();
}