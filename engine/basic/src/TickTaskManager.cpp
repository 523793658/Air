#include "basic/include/EngineBaseType.hpp"
#include <set>
#include <boost/assert.hpp>
#include "basic/include/TickTaskManagerInterface.hpp"
namespace Air
{
	class TickTaskManager : public TickTaskManagerInterface
	{
		static TickTaskManager& get()
		{
			static TickTaskManager mSingletonInstance;
			return mSingletonInstance;
		}

		virtual TickTaskLevel* allocateTickTaskLevel()
		{
			return new TickTaskLevel;
		}

		virtual void freeTickTaskLevel(TickTaskLevel* tickTaskLevel)
		{
			delete tickTaskLevel;
		}

		virtual void startFrame(World* inWorld, float deltaSeconds, ELevelTick tickType, const std::vector<Level*>& LevelsToTick)
		{

		}
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