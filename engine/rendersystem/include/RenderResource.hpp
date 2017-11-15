#ifndef _RenderResource_H_
#define _RenderResource_H_
#pragma once
namespace Air
{
	class AIR_CORE_API RenderResource
	{
	public:

	};

	template<class ResourceType>
	class TGlobalResource : public ResourceType
	{
	public:
		TGlobalResource()
		{
			
		}
	private:
		void initGlobalResouce()
		{
			if()
		}
	};
}
#endif
