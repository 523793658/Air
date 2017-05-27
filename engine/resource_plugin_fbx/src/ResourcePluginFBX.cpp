#include "ResourcePluginFBX.h"
namespace Air
{

	ResourcePluginFBX::ResourcePluginFBX() : ResourcePlugin("ResourcePluginFBX", "fbx")
	{

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