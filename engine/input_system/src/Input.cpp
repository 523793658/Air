#include "Context.h"
#include "input_system/include/Input.hpp"

namespace Air
{
	void InputActionMap::addAction(InputActionDefine const & action_define)
	{
		mActionMap.emplace(action_define.semantic, action_define.action);
	}

	void InputActionMap::updateInputActions(InputActionsType& actions, uint16_t key, InputActionParamPtr const & param)
	{
		if (this->hasAction(key))
		{
			actions.emplace_back(this->getAction(key), param);
		}
	}


	bool InputActionMap::hasAction(uint16_t key) const
	{
		return (mActionMap.find(key) != mActionMap.end());
	}
	uint16_t InputActionMap::getAction(uint16_t key) const
	{
		return mActionMap.find(key)->second;
	}

	InputEngine::~InputEngine()
	{

	}

	void InputEngine::update()
	{
		mElapsedTime = static_cast<float>(mTimer.getElapsed());
		if (mElapsedTime > 0.01f)
		{
			mTimer.restart();
			for (auto const & device : mDevices)
			{
				device->updateInputs();
			}
			for (uint32_t id = 0; id < mActionHandlers.size(); ++id)
			{
				boost::container::flat_map<uint16_t, InputActionParamPtr> actions;
				for (auto const & device : mDevices)
				{
					InputActionsType const theAction(device->updateActionMap(id));
					for (auto const & act : theAction)
					{
						if (actions.find(act.first) == actions.end())
						{
							actions.insert(act);
							(*mActionHandlers[id].second)(*this, act);
						}
					}
				}
			}
		}
	}

	void InputEngine::suspend()
	{
		this->doSuspend();
	}
	void InputEngine::resume()
	{
		this->doResume();
	}
	

	float InputEngine::elapsedTime() const
	{
		return mElapsedTime;
	}

	void InputEngine::actionMap(InputActionMap const  actionMap, ActionHandlerT handler)
	{
		mActionHandlers.emplace_back(actionMap, handler);
		if (mDevices.empty())
		{
			this->enumDevices();
		}
		for (uint32_t id = 0; id < mActionHandlers.size(); ++id)
		{
			for (auto const & device : mDevices)
			{
				device->actionMap(id, mActionHandlers[id].first);
			}
		}
	}

	size_t InputEngine::getNumDevice() const
	{
		return mDevices.size();
	}
	InputDevicePtr InputEngine::getDevice(size_t index) const
	{
		return mDevices[index];
	}

	InputDevice::~InputDevice()
	{

	}

	InputKeyboard::InputKeyboard()
		:mIndex(false), mActionParam(MakeSharedPtr<InputKeyboardActionParam>())
	{
		mKeys[0].fill(false);
		mKeys[1].fill(false);
		mActionParam->type = InputEngine::IDT_Keyboard;
	}
	InputKeyboard::~InputKeyboard()
	{

	}

	bool InputKeyboard::getKey(size_t n) const
	{
		BOOST_ASSERT(n < mKeys[mIndex].size());
		return mKeys[mIndex][n];
	}

	bool const * InputKeyboard::getKeys() const
	{
		return &mKeys[mIndex][0];
	}

	bool InputKeyboard::isKeyDown(size_t n) const
	{
		BOOST_ASSERT(n < mKeys[mIndex].size());
		return (mKeys[mIndex][n] && !mKeys[!mIndex][n]);
	}

	bool InputKeyboard::isKeyUp(size_t n) const

	{
		BOOST_ASSERT(n < mKeys[mIndex].size());
		return (!mKeys[mIndex][n] && mKeys[!mIndex][n]);
	}

	void InputKeyboard::actionMap(uint32_t id, InputActionMap const & actionMap)
	{
		InputActionMap& iam = mActionMaps[id];
		for (uint16_t i = KS_Escape; i <= KS_AnyKey; ++i)
		{
			if (actionMap.hasAction(i))
			{
				iam.addAction(InputActionDefine(actionMap.getAction(i), i));
			}
		}
	}

	InputActionsType InputKeyboard::updateActionMap(uint32_t id)
	{
		InputActionsType ret;
		InputActionMap & iam = mActionMaps[id];
		for (uint16_t i = 0; i < this->getNumKeys(); ++i)
		{
			mActionParam->mButtonsState[i] = this->getKey(i);
			mActionParam->mButtonsDown[i] = this->isKeyDown(i);
			mActionParam->mButtonsUp[i] = this->isKeyUp(i);
		}
		bool any_key = false;
		for (uint16_t i = 0; i < this->getNumKeys(); ++i)
		{
			if (mKeys[mIndex][i] || mKeys[!mIndex][i])
			{
				iam.updateInputActions(ret, i, mActionParam);
				any_key = true;
			}
		}
		if (any_key)
		{
			iam.updateInputActions(ret, KS_AnyKey, mActionParam);
		}
		return ret;
	}

	InputMouse::InputMouse()
		: mAbsPos(0, 0), mOffset(0, 0, 0), mNumButtons(0), mIndex(false), mShiftCtrlAlt(0), mActionParam(MakeSharedPtr<InputMouseActionParam>())
	{
		mButtons[0].fill(false);
		mButtons[1].fill(false);
		mActionParam->type = InputEngine::IDT_Mouse;
	}
	InputMouse::~InputMouse()
	{

	}

	long InputMouse::getAbsX() const
	{
		return mAbsPos.x();
	}

	long InputMouse::getAbsY() const
	{
		return mAbsPos.y();
	}

	long InputMouse::getX() const
	{
		return mOffset.x();
	}
	long InputMouse::getY() const
	{
		return mOffset.y();
	}
	long InputMouse::getZ() const
	{
		return mOffset.z();
	}

	bool InputMouse::isLeftButton() const
	{
		return this->getButton(0);
	}

	bool InputMouse::isRightButton() const
	{
		return this->getButton(1);
	}
	bool InputMouse::isMiddleButton() const
	{
		return this->getButton(2);
	}

	size_t InputMouse::getNumButtons() const
	{
		return mNumButtons;
	}

	bool InputMouse::getButton(size_t n) const
	{
		BOOST_ASSERT(n < mButtons[mIndex].size());
		return mButtons[mIndex][n];
	}

	bool InputMouse::isButtonDown(size_t n) const
	{
		BOOST_ASSERT(n < mButtons[mIndex].size());
		return (mButtons[mIndex][n] && !mButtons[!mIndex][n]);
	}
	bool InputMouse::isButtonUp(size_t n) const
	{
		BOOST_ASSERT(n < mButtons[mIndex].size());
		return (!mButtons[mIndex][n] && mButtons[!mIndex][n]);
	}

	void InputMouse::actionMap(uint32_t id, InputActionMap const & actionMap)
	{
		InputActionMap & iam = mActionMaps[id];
		for (uint16_t i = MS_X; i <= MS_AnyButton; ++i)
		{
			if (actionMap.hasAction(i))
			{
				iam.addAction(InputActionDefine(actionMap.getAction(i), i));
			}
		}
	}

	InputActionsType InputMouse::updateActionMap(uint32_t id)
	{
		InputActionsType ret;
		InputActionMap& iam = mActionMaps[id];
		mActionParam->mMoveVec = int2(mOffset.x(), mOffset.y());
		mActionParam->mWheelDelta = mOffset.z();
		mActionParam->mAbsCoord = mAbsPos;
		mActionParam->mButtonsState = 0;
		mActionParam->mButtonsUp = mShiftCtrlAlt;
		mActionParam->mButtonsDown = 0;
		for (size_t i = 0; i < this->getNumButtons(); ++i)
		{
			mActionParam->mButtonsState |= (this->getButton(i) ? (1UL << i) : 0);
			mActionParam->mButtonsDown |= (this->isButtonDown(i) ? (1UL << i) : 0);
			mActionParam->mButtonsUp |= (this->isButtonUp(i) ? (1UL << i) : 0);
		}
		if (mOffset.x() != 0)
		{
			iam.updateInputActions(ret, MS_X, mActionParam);
		}
		if (mOffset.y() != 0)
		{
			iam.updateInputActions(ret, MS_Y, mActionParam);
		}
		if (mOffset.z() != 0)
		{
			iam.updateInputActions(ret, MS_Z, mActionParam);
		}
		bool any_button = false;
		for (uint16_t i = 0; i < this->getNumButtons(); ++i)
		{
			if (mButtons[mIndex][i] || mButtons[!mIndex][i])
			{
				iam.updateInputActions(ret, static_cast<uint16_t>(MS_Button0 + i), mActionParam);
				any_button = true;
			}
		}
		if (any_button)
		{
			iam.updateInputActions(ret, MS_AnyButton, mActionParam);
		}
		return ret;
	}

}
