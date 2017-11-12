#ifndef _CommandList_H_
#define _CommandList_H_
#pragma once
namespace Air
{
	class AIR_CORE_API CommandListBase : boost::noncopyable
	{
	public:
		mutable uint32_t mStrictGraphicsPipelineStateUse;

		CommandListBase();
		~CommandListBase();

		void * operator new(size_t size);

		void operator delete(void * rawMenory);

	};

	class AIR_CORE_API CommandList : public CommandListBase
	{
	public:

	};

	class AIR_CORE_API CommandListImmediate : public CommandList
	{

	};




}

#endif
