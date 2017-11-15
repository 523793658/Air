#include "Context.h"
#include "rendersystem/include/UniformBuffer.hpp"


namespace Air
{
	std::unordered_map<std::string, UniformBufferStruct*>& UniformBufferStruct::getNameStructMap()
	{
		static std::unordered_map<std::string, UniformBufferStruct*> mGlobalNameStructMap;
		return mGlobalNameStructMap;
	}


	std::list<UniformBufferStruct*> *& UniformBufferStruct::getStructList()
	{
		static std::list<UniformBufferStruct*>* GUniformStructList = nullptr;
		return GUniformStructList;
	}

}