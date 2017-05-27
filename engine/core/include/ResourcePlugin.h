#ifndef _Resource_Plugin_H_
#define _Resource_Plugin_H_
#include "core/include/Engine.h"
namespace Air
{

	class AIR_CORE_API ResourcePlugin
	{
	public:
		ResourcePlugin(std::string name, std::string format);
		virtual void importResource(ResLoadingDescPtr& res_desc) = 0;
		virtual void exportResource(ResLoadingDescPtr& res_desc) = 0;
	private:
		std::string name;
		std::string format;
	};





}









#endif