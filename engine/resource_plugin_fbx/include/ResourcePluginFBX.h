#ifndef _Resource_Plugins_FBX_H_
#define _Resource_Plugins_FBX_H_
#include "fbxsdk.h"


#include "basic/include/Config.h"
#include "Engine.h"


#include "ResourcePlugin.h"
#ifdef AIR_FBX_SOURCE
#define AIR_FBX_API AIR_SYMBOL_EXPORT
#else
#define AIR_FBX_API AIR_SYMBOL_IMPORT
#endif

extern "C"
{
	AIR_FBX_API void makeResourcePlugin(std::unique_ptr<Air::ResourcePlugin>& ptr);
}


namespace Air
{
	class ResourcePluginFBX : public ResourcePlugin
	{
	public:
		ResourcePluginFBX();



	public:
		virtual void importResource(ResLoadingDescPtr& res_desc);
		virtual void exportResource(ResLoadingDescPtr& res_desc);
	private:

		FbxManager* mSDKManager;
	};




}


#endif