#include "basic/include/EngineBaseType.hpp"
#include <set>
#include <boost/assert.hpp>
#include "core/include/World.hpp"
#include "core/include/Level.hpp"
#include "basic/include/TickTaskManagerInterface.hpp"
namespace Air
{
	struct TickContext
	{
		float mDeltaSeconds;
		ELevelTick	mTickType;
		ETickingGroup		mTickGroup;
		World*				mWorld;
		TickContext(float inDeltaSeconds = 0.0f, ELevelTick inTickType = ELT_All, ETickingGroup tickGroup = TG_PrePhysics)
			:mDeltaSeconds(inDeltaSeconds),
			mTickType(inTickType),
			mTickGroup(tickGroup),
			mWorld(nullptr)
		{
		}
		TickContext(const TickContext& In)
			: mDeltaSeconds(In.mDeltaSeconds),
			mTickType(In.mTickType),
			mTickGroup(In.mTickGroup),
			mWorld(In.mWorld)
		{
		}

		void operator = (const TickContext& rhs)
		{
			mDeltaSeconds = rhs.mDeltaSeconds;
			mTickGroup = rhs.mTickGroup;
			mTickType = rhs.mTickType;
			mWorld = rhs.mWorld;
		}
	};

	class TickTaskSequencer
	{
		class DispatchTickGroupTask
		{
			TickTaskSequencer & mTTS;
			ETickingGroup		mWorldTickGroup;
		public:
			FORCEINLINE DispatchTickGroupTask(TickTaskSequencer& inTTS, ETickingGroup inWorldTickGroup)
				:mTTS(inTTS),
				mWorldTickGroup(inWorldTickGroup)
			{

			}
		};
	public:
		static TickTaskSequencer& get()
		{
			static TickTaskSequencer mSingletonInstance;
			return mSingletonInstance;
		}
		void startFrame();

		void setupAddTickTaskCompletionParallel(int32_t NumTicks);
	};

	class TickTaskManager : public TickTaskManagerInterface
	{
	private:
	public:

		static TickTaskManager& get()
		{
			//static TickTaskManager mSingletonInstance;
			//return mSingletonInstance;
		}

		virtual TickTaskLevel* allocateTickTaskLevel()
		{
			return new TickTaskLevel;
		}

		void fillLevelList(const std::vector<Level*>& levels)
		{
			if (!mContext.mWorld->getActiveLevelCollection() || mContext.mWorld->getActiveLevelCollection()->getType() == LevelCollectionType::DynamicSourceLevels)
			{
				mLevelList.push_back(mContext.mWorld->mTickTaskLevel);
			}
			for (int32_t levelIndex = 0; levelIndex < levels.size(); ++levelIndex)
			{
				Level* level = levels[levelIndex];
				if (level->mIsVisible)
				{
					mLevelList.push_back(level->mTickTaskLevel);
				}
			}
		}

		virtual void freeTickTaskLevel(TickTaskLevel* tickTaskLevel)
		{
			delete tickTaskLevel;
		}

		virtual void startFrame(World* inWorld, float deltaSeconds, ELevelTick tickType, const std::vector<Level*>& LevelsToTick)
		{
			mContext.mTickGroup = ETickingGroup(0);
			mContext.mDeltaSeconds = deltaSeconds;
			mContext.mTickType = tickType;
			mContext.mWorld = inWorld;
			mTickNewlySpawned = true;
			mTickTaskSequencer.startFrame();
			fillLevelList(LevelsToTick);
			int32_t numWorkerThread = 0;
			bool bConcurrentQueue = false;
			if (!bConcurrentQueue)
			{
				int32_t totalTickFunctions = 0;
				for (int32_t levelIndex = 0; levelIndex < mLevelList.size(); ++levelIndex)
				{
					totalTickFunctions += mLevelList[levelIndex]->startFrame(mContext);
				}
				for (int32_t levelIndex = 0; levelIndex < mLevelList.size(); ++levelIndex)
				{
					mLevelList[levelIndex]->queueAllTicks();
				}
			}
			else
			{
				for (int32_t levelIndex = 0; levelIndex < mLevelList.size(); ++levelIndex)
				{
					mLevelList[levelIndex]->startFrameParallel(mContext, mAllTickFunctions);
				}
				TickTaskSequencer& tts = TickTaskSequencer::get();
				tts.setupAddTickTaskCompletionParallel(mAllTickFunctions.size());
				for (int32_t levelIndex = 0; levelIndex < mLevelList.size(); levelIndex++)
				{
					//mLevelList[levelIndex]->reser
				}
			}
		}

	private:
		TickContext		mContext;

		float			mTickNewlySpawned;
		TickTaskSequencer&		mTickTaskSequencer;
		std::vector<TickTaskLevel*> mLevelList;
		std::vector<TickFunction*> mAllTickFunctions;
	};

	class TickTaskLevel
	{
	public:
		TickTaskLevel()
		{

		}

		void removeTickFunction(TickFunction* tickFunction)
		{
			switch (tickFunction->mTickState)
			{
			case TickFunction::ETickState::Enabled:
				if (tickFunction->mTickInterval > 0.f)
				{
					if (mAllEnabledTickFunction.erase(tickFunction) == 0)
					{
						TickFunction* prevComparisionFunction = nullptr;
						TickFunction* comparisonFunction = mAllCoolingDownTickFunctions.mHead;
						bool bFound = false;
						while (comparisonFunction && !bFound)
						{
							if (comparisonFunction == tickFunction)
							{
								bFound = true;
								if (prevComparisionFunction)
								{
									prevComparisionFunction->mNext = tickFunction->mNext;
								}
								else
								{
									mAllCoolingDownTickFunctions.mHead = tickFunction->mNext;
								}
								tickFunction->mNext = nullptr;
							}
							else
							{
								prevComparisionFunction = comparisonFunction;
								comparisonFunction = comparisonFunction->mNext;
							}
						}
					}
				}
				else
				{
					BOOST_VERIFY(mAllEnabledTickFunction.erase(tickFunction) == 1);
				}
				break;
			case TickFunction::ETickState::Disabled:
				BOOST_VERIFY(mAllEnabledTickFunction.erase(tickFunction) == 1);
				break;
			case TickFunction::ETickState::CoolingDown:
				TickFunction* prevComparisionFunction = nullptr;
				TickFunction* comparisonFunction = mAllCoolingDownTickFunctions.mHead;
				bool bFound = false;
				while (comparisonFunction && !bFound)
				{
					if (comparisonFunction == tickFunction)
					{
						bFound = true;
						if (prevComparisionFunction)
						{
							prevComparisionFunction->mNext = tickFunction->mNext;
						}
						else
						{
							mAllCoolingDownTickFunctions.mHead = tickFunction->mNext;
						}
						if (tickFunction->mNext)
						{
							tickFunction->mNext->mRelativeTickCoolDown += tickFunction->mRelativeTickCoolDown;
							tickFunction->mNext = nullptr;
						}
					}
					else
					{
						prevComparisionFunction = comparisonFunction;
						comparisonFunction = comparisonFunction->mNext;
					}
				}
				break;
			}
			if (mTickNewlySpawned)
			{
				mNewlySpawnedTickFunctions.erase(tickFunction);
			}
		}
		void addTickFunction(TickFunction* tickFunction)
		{
			if (tickFunction->mTickState == TickFunction::ETickState::Enabled)
			{
				mAllEnabledTickFunction.emplace(tickFunction);
				if (mTickNewlySpawned)
				{
					mNewlySpawnedTickFunctions.emplace(tickFunction);
				}
			}
			else
			{
				mAllDisabledTickFunctions.emplace(tickFunction);
			}
		}
		int32_t startFrame(const TickContext& context);
		void queueAllTicks();

		void startFrameParallel(const TickContext& context, std::vector<TickFunction*>& allTickFunctions);

		void reserveTickFunctionCooldowns(int32_t numToReserve)
		{
			
		}
	private:
		struct CoolingDownTickFunctionList
		{
			CoolingDownTickFunctionList()
				:mHead(nullptr)
			{

			}

			bool contains(TickFunction* tickFunction) const
			{
				TickFunction* node = mHead;
				while (node)
				{
					if (node == tickFunction)
					{
						return true;
					}
					node = node->mNext;
				}
				return false;
			}

			TickFunction* mHead;
		};


		CoolingDownTickFunctionList			mAllCoolingDownTickFunctions;
		std::set<TickFunction*>				mAllEnabledTickFunction;
		std::set<TickFunction*>				mNewlySpawnedTickFunctions;
		std::set<TickFunction*>				mAllDisabledTickFunctions;
		bool								mTickNewlySpawned;
	};


	void TickFunction::setTickFunctionEnable(bool bInEnable)
	{
		if (mRegistered && (bInEnable == (mTickState == ETickState::Enabled)))
		{
			mTickTaskLevel->removeTickFunction(this);
			mTickState = (bInEnable ? ETickState::Enabled : ETickState::Disabled);
			mTickTaskLevel->addTickFunction(this);
		}
		else
		{
			mTickState = (bInEnable ? ETickState::Enabled : ETickState::Disabled);
		}
	}

	void TickFunction::unRegisterTickFunction()
	{
		if (mRegistered)
		{
			
		}
	}
}