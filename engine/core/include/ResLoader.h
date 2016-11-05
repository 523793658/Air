#ifndef _Core_ResLoader_H_
#define _Core_ResLoader_H_
#pragma once


#include <basic/include/PreDeclare.h>
#include <istream>
#include <vector>
#include <string>
#if defined(AIR_COMPILER_MSVC)
#pragma warning(push)
#pragma warning(disable: 4512)
#elif defined(AIR_COMPILER_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored
#endif

#if defined(AIR_COMPILER_MSVC)
#pragma warning(pop)
#elif defined(AIR_COMPILER_CLANG)
#pragma clang diagnostic pop
#endif

#include "basic\include\ResIdentifier.h"
#include "basic\include\Thread.h"

namespace Air
{
	class AIR_CORE_API ResLoadingDesc
	{
	public:
		virtual ~ResLoadingDesc(){}
		virtual uint64_t getType() const = 0;
		virtual bool stateLess() const = 0;
		//virtual std::shared_ptr<void> createResource();

		virtual void subThreadStage() = 0;
		virtual std::shared_ptr<void> mainThreadStage() = 0;

		virtual bool hasSubThreadStage() const = 0;

		virtual bool match(ResLoadingDesc const & rhs) const = 0;
		virtual std::shared_ptr<void> cloneResourceFrom(std::shared_ptr<void> const& resource) = 0;

		virtual std::shared_ptr<void> getResource() const = 0;
	};
	class AIR_CORE_API ResLoader
	{
	public:

		ResLoader();
		~ResLoader();

		static ResLoader& getInstance();
		static void destroy();

		void suspend();
		void resume();

		void addPath(std::string const & path);
		void delPath(std::string const & path);

		ResIdentifierPtr open(std::string const & name);
		std::string locate(std::string const & name);
		ResIdentifierPtr LocatePkt(std::string const & name, std::string const & res_name, std::string& password, std::string& internal_name);
	private:
		static std::unique_ptr<ResLoader> mInstance;

		enum LoadingStatus
		{
			LS_Loading,
			LS_Complete,
			LS_CanBeRemoved
		};

		std::string mExePath;
		std::vector<std::string> mPaths;
		std::mutex pathsMutex;

		std::mutex loadedMutex;
		std::mutex loadingMutex;


	};
}







#endif