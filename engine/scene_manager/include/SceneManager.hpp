#ifndef _SceneManager_H_
#define _SceneManager_H_
#include <unordered_map>
#include "boost/noncopyable.hpp"
#include "mutex"
#include "basic/include/Thread.h"
namespace Air
{
	class AIR_CORE_API SceneManager : boost::noncopyable
	{
	public:
		SceneManager();
		virtual ~SceneManager();

// 		void suspend();
// 		void resume();
// 
// 		void setSmallObjectThreshold(float area);
// 		void sceneObjectElapse(float elapse);
// 		virtual void clipScene();
// 
// 		void addCamera(CameraPtr const & camera);
// 		void delCamera(CameraPtr const & camera);
// 
// 		uint32_t getNumCameras() const;
// 		CameraPtr& getCamera(uint32_t index);
// 		CameraPtr const & getCamera(uint32_t index) const;

		void addSceneObjectLocked(SceneObjectPtr const & obj);

		void update();
	protected:
		void flush(uint32_t urt);
	private:
		void flushScene();

	private:
		uint32_t mURT;
		uint32_t mNumObjectsRendered;
		uint32_t mNumRenderablesRendered;
		uint32_t mNumPrimitivesRendered;
		uint32_t mNumVerticesRendered;
		uint32_t mNumDrawCalls;
		uint32_t mNumDispatchCalls;
		std::mutex mUpdateMutex;
		std::unique_ptr<joiner<void>> mUpdateThread;
		volatile bool mQuit;



		bool mDeferredMode;
	protected:
		std::unordered_map<size_t, std::shared_ptr<std::vector<BoundOverlap>>> mVisibleMarksMap;
		std::vector<SceneObjectPtr> mSceneObjs;
		std::vector<SceneObjectPtr> mOverlaySceneObjs;
		float mSmallObjThreshold;
		float mUpdateElapse;
	};
}

#endif // !_SceneManager_H_
