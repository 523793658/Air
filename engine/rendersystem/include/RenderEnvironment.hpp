#ifndef _RenderEnvironment_H_
#define _RenderEnvironment_H_
#pragma once
#include "PreDeclare.h"
#include <unordered_map>
#include "Event.hpp"
#include "rendersystem/include/RenderEffect.hpp"
namespace Air
{
	class AIR_CORE_API RenderEnvironment : boost::noncopyable, public EventTable
	{

	public:
		static const size_t RE_EVENT_UPDATE_DIRECTION_LIGHT;

	public:
		RenderEnvironment();

		SharedConstantBuffer* const getConstantBuffer(std::string name) const;

		SharedConstantBuffer* addConstantBuffer(std::string name, ConstantBufferType type);

		void updateLights(std::vector<LightSourcePtr> const & lights);

		void update();
	private:

		std::unordered_map<std::string, std::unique_ptr<SharedConstantBuffer>> mSharedConstanBuffers;
	};



}


#endif
