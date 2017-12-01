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

#include <boost/lockfree/spsc_queue.hpp>

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
		virtual bool getStateLess() const = 0;
		//virtual std::shared_ptr<void> createResource();

		virtual void subThreadStage() = 0;
		virtual void mainThreadStage() = 0;

		virtual std::shared_ptr<void> createResource()
		{
			return std::shared_ptr<void>();
		}

		virtual bool hasSubThreadStage() const = 0;

		virtual bool match(ResLoadingDesc const & rhs) const = 0;
		virtual void copyDataFrom(ResLoadingDesc const & rhs) = 0;
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

		std::wstring getAbsPath(std::wstring const & path);

		ResIdentifierPtr open(std::string const & name);
		std::string locate(std::string const & name);
		ResIdentifierPtr LocatePkt(std::string const & name, std::string const & res_name, std::string& password, std::string& internal_name);

		void update();


		std::shared_ptr<void> syncQuery(ResLoadingDescPtr const & res_desc);

		std::shared_ptr<void> aSyncQuery(ResLoadingDescPtr const & res_desc);

		template<typename T>
		std::shared_ptr<T> syncQueryT(ResLoadingDescPtr const & res_desc)
		{
			return std::static_pointer_cast<T>(this->syncQuery(res_desc));
		}

		template<typename T>
		std::shared_ptr<T> aSyncQueryT(ResLoadingDescPtr const & res_desc)
		{
			return std::static_pointer_cast<T>(this->aSyncQuery(res_desc));
		}
	private:
		std::wstring getRealPath(std::wstring const & path);
		void loadingThreadFunc();
		void removeUnrefResources();
		std::shared_ptr<void> findMatchLoadedResource(ResLoadingDescPtr const & res_desc);

		void addLoadedResource(ResLoadingDescPtr const & res_desc, std::shared_ptr<void> const & res);
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

		std::mutex mLoadedMutex;
		std::mutex mLoadingMutex;

		std::vector<std::pair<ResLoadingDescPtr, std::weak_ptr<void>>> mLoadedRes;
		std::vector<std::pair<ResLoadingDescPtr, std::shared_ptr<volatile LoadingStatus>>> mLoadingRes;
		boost::lockfree::spsc_queue<std::pair<ResLoadingDescPtr, std::shared_ptr<volatile LoadingStatus>>, boost::lockfree::capacity<1024>> mLoadingResQueue;

		std::unique_ptr<joiner<void>> mLoadingThread;
		volatile bool mQuit;
	};
}







#endif