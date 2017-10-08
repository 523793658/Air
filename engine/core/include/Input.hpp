#ifndef _Input_H_
#define _Input_H_
#pragma once


#include "PreDeclare.h"
#include "basic/include/Vector.hpp"
#include "basic/include/Timer.hpp"

#if defined(AIR_COMPILER_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(AIR_COMPILER_GCC)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

#include "boost/container/flat_map.hpp"
#if defined(AIR_COMPILER_CLANG)
#pragma clang diagnostic pop
#elif defined(AIR_COMPILER_GCC)
#pragma GCC diagnostic pop
#endif

#if defined(AIR_COMPILER_MSVC)
#pragma warning(push)
#pragma warning(disable: 4512)
#pragma warning(disable: 4913)
#elif defined(AIR_COMPILER_GCC)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(AIR_COMPILER_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif
#include "boost/signals2.hpp"
#if defined(AIR_COMPILER_MSVC)
#pragma warning(pop)
#elif defined(AIR_COMPILER_GCC)
#pragma GCC diagnostic pop
#elif defined(AIR_COMPILER_CLANG)
#pragma clang diagnostic pop
#endif

#include <vector>
#include <string>
#include <bitset>
#include <array>

namespace Air
{
	enum KeyboardSemantic
	{
		KS_Escape = 0x01,
		KS_1 = 0x02,
		KS_2 = 0x03,
		KS_3 = 0x04,
		KS_4 = 0x05,
		KS_5 = 0x06,
		KS_6 = 0x07,
		KS_7 = 0x08,
		KS_8 = 0x09,
		KS_9 = 0x0A,
		KS_0 = 0x0B,
		KS_Minus = 0x0C,		// - on main keyboard
		KS_Equals = 0x0D,
		KS_BackSpace = 0x0E,		// backspace
		KS_Tab = 0x0F,
		KS_Q = 0x10,
		KS_W = 0x11,
		KS_E = 0x12,
		KS_R = 0x13,
		KS_T = 0x14,
		KS_Y = 0x15,
		KS_U = 0x16,
		KS_I = 0x17,
		KS_O = 0x18,
		KS_P = 0x19,
		KS_LeftBracket = 0x1A,
		KS_RightBracket = 0x1B,
		KS_Enter = 0x1C,		// Enter on main keyboard
		KS_LeftCtrl = 0x1D,
		KS_A = 0x1E,
		KS_S = 0x1F,
		KS_D = 0x20,
		KS_F = 0x21,
		KS_G = 0x22,
		KS_H = 0x23,
		KS_J = 0x24,
		KS_K = 0x25,
		KS_L = 0x26,
		KS_Semicolon = 0x27,
		KS_Apostrophe = 0x28,
		KS_Grave = 0x29,		// accent grave
		KS_LeftShift = 0x2A,
		KS_BackSlash = 0x2B,
		KS_Z = 0x2C,
		KS_X = 0x2D,
		KS_C = 0x2E,
		KS_V = 0x2F,
		KS_B = 0x30,
		KS_N = 0x31,
		KS_M = 0x32,
		KS_Comma = 0x33,
		KS_Period = 0x34,		// . on main keyboard
		KS_Slash = 0x35,		// / on main keyboard
		KS_RightShift = 0x36,
		KS_NumPadStar = 0x37,		// * on numeric keypad
		KS_LeftAlt = 0x38,
		KS_Space = 0x39,
		KS_CapsLock = 0x3A,
		KS_F1 = 0x3B,
		KS_F2 = 0x3C,
		KS_F3 = 0x3D,
		KS_F4 = 0x3E,
		KS_F5 = 0x3F,
		KS_F6 = 0x40,
		KS_F7 = 0x41,
		KS_F8 = 0x42,
		KS_F9 = 0x43,
		KS_F10 = 0x44,
		KS_NumLock = 0x45,
		KS_ScrollLock = 0x46,
		KS_NumPad7 = 0x47,
		KS_NumPad8 = 0x48,
		KS_NumPad9 = 0x49,
		KS_NumPadMinus = 0x4A,		// - on numeric keypad
		KS_NumPad4 = 0x4B,
		KS_NumPad5 = 0x4C,
		KS_NumPad6 = 0x4D,
		KS_NumPadPlus = 0x4E,		// + on numeric keypad
		KS_NumPad1 = 0x4F,
		KS_NumPad2 = 0x50,
		KS_NumPad3 = 0x51,
		KS_NumPad0 = 0x52,
		KS_NumPadPeriod = 0x53,		// . on numeric keypad
		KS_OEM_102 = 0x56,		// <> or \| on RT 102-key keyboard (Non-U.S.)
		KS_F11 = 0x57,
		KS_F12 = 0x58,
		KS_F13 = 0x64,		//                     (NEC PC98)
		KS_F14 = 0x65,		//                     (NEC PC98)
		KS_F15 = 0x66,		//                     (NEC PC98)
		KS_Kana = 0x70,		// (Japanese keyboard)
		KS_ABNT_C1 = 0x73,		// /? on Brazilian keyboard
		KS_Convert = 0x79,		// (Japanese keyboard)
		KS_NoConvert = 0x7B,		// (Japanese keyboard)
		KS_Yen = 0x7D,		// (Japanese keyboard)
		KS_ABNT_C2 = 0x7E,		// Numpad . on Brazilian keyboard
		KS_NumPadEquals = 0x8D,		// = on numeric keypad (NEC PC98)
		KS_PrevTrack = 0x90,		// Previous Track (DKS_CIRCUMFLEX on Japanese keyboard)
		KS_AT = 0x91,		//                     (NEC PC98)
		KS_Colon = 0x92,		//                     (NEC PC98)
		KS_Underline = 0x93,		//                     (NEC PC98)
		KS_Kanji = 0x94,		// (Japanese keyboard)
		KS_Stop = 0x95,		//                     (NEC PC98)
		KS_AX = 0x96,		//                     (Japan AX)
		KS_Unlabeled = 0x97,		//                        (J3100)
		KS_NextTrack = 0x99,		// Next Track
		KS_NumPadEnter = 0x9C,		// Enter on numeric keypad
		KS_RightCtrl = 0x9D,
		KS_Mute = 0xA0,		// Mute
		KS_Calculator = 0xA1,		// Calculator
		KS_PlayPause = 0xA2,		// Play / Pause
		KS_MediaStop = 0xA4,		// Media Stop
		KS_VolumeDown = 0xAE,		// Volume -
		KS_VolumeUp = 0xB0,		// Volume +
		KS_WebHome = 0xB2,		// Web home
		KS_NumPadComma = 0xB3,		// , on numeric keypad (NEC PC98)
		KS_NumPadSlash = 0xB5,		// / on numeric keypad
		KS_SysRQ = 0xB7,
		KS_RightAlt = 0xB8,		// right Alt
		KS_Pause = 0xC5,		// Pause
		KS_Home = 0xC7,		// Home on arrow keypad
		KS_UpArrow = 0xC8,		// UpArrow on arrow keypad
		KS_PageUp = 0xC9,		// PgUp on arrow keypad
		KS_LeftArrow = 0xCB,		// LeftArrow on arrow keypad
		KS_RightArrow = 0xCD,		// RightArrow on arrow keypad
		KS_End = 0xCF,		// End on arrow keypad
		KS_DownArrow = 0xD0,		// DownArrow on arrow keypad
		KS_PageDown = 0xD1,		// PgDn on arrow keypad
		KS_Insert = 0xD2,		// Insert on arrow keypad
		KS_Delete = 0xD3,		// Delete on arrow keypad
		KS_LeftWin = 0xDB,		// Left Windows key
		KS_RightWin = 0xDC,		// Right Windows key
		KS_Apps = 0xDD,		// AppMenu key
		KS_Power = 0xDE,		// System Power
		KS_Sleep = 0xDF,		// System Sleep
		KS_Wake = 0xE3,		// System Wake
		KS_WebSearch = 0xE5,		// Web Search
		KS_WebFavorites = 0xE6,		// Web Favorites
		KS_WebRefresh = 0xE7,		// Web Refresh
		KS_WebStop = 0xE8,		// Web Stop
		KS_WebForward = 0xE9,		// Web Forward
		KS_WebBack = 0xEA,		// Web Back
		KS_MyComputer = 0xEB,		// My Computer
		KS_Mail = 0xEC,		// Mail
		KS_MediaSelect = 0xED,		// Media Select

		KS_AnyKey
	};

	// 鼠标动作
	enum MouseSemantic
	{
		MS_X = 0x100,
		MS_Y = 0x101,
		MS_Z = 0x102,
		MS_Button0 = 0x103,
		MS_Button1 = 0x104,
		MS_Button2 = 0x105,
		MS_Button3 = 0x106,
		MS_Button4 = 0x107,
		MS_Button5 = 0x108,
		MS_Button6 = 0x109,
		MS_Button7 = 0x10A,

		MS_AnyButton
	};

	// 游戏杆动作
	enum JoystickSemantic
	{
		JS_XPos = 0x200,
		JS_YPos = 0x201,
		JS_ZPos = 0x202,
		JS_XRot = 0x203,
		JS_YRot = 0x204,
		JS_ZRot = 0x205,
		JS_Slider0 = 0x206,
		JS_Slider1 = 0x207,
		JS_Button0 = 0x208,
		JS_Button1 = 0x209,
		JS_Button2 = 0x20A,
		JS_Button3 = 0x20B,
		JS_Button4 = 0x20C,
		JS_Button5 = 0x20D,
		JS_Button6 = 0x20E,
		JS_Button7 = 0x20F,
		JS_Button8 = 0x210,
		JS_Button9 = 0x211,
		JS_Button10 = 0x212,
		JS_Button11 = 0x213,
		JS_Button12 = 0x214,
		JS_Button13 = 0x215,
		JS_Button14 = 0x216,
		JS_Button15 = 0x217,
		JS_Button16 = 0x218,
		JS_Button17 = 0x219,
		JS_Button18 = 0x21A,
		JS_Button19 = 0x21B,
		JS_Button20 = 0x21C,
		JS_Button21 = 0x21D,
		JS_Button22 = 0x21E,
		JS_Button23 = 0x21F,
		JS_Button24 = 0x220,
		JS_Button25 = 0x221,
		JS_Button26 = 0x222,
		JS_Button27 = 0x223,
		JS_Button28 = 0x224,
		JS_Button29 = 0x225,
		JS_Button30 = 0x226,
		JS_Button31 = 0x227,

		JS_AnyButton
	};

	enum TouchSemantic
	{
		TS_None = 0x300,
		TS_Pan = 0x301,
		TS_Tap = 0x302,
		TS_Press = 0x303,
		TS_PressAndTap = 0x304,
		TS_Zoom = 0x305,
		TS_Rotate = 0x306,
		TS_Flick = 0x307,
		TS_Wheel = 0x308,
		TS_Touch0 = 0x309,
		TS_Touch1 = 0x30A,
		TS_Touch2 = 0x30B,
		TS_Touch3 = 0x30C,
		TS_Touch4 = 0x30D,
		TS_Touch5 = 0x30E,
		TS_Touch6 = 0x30F,
		TS_Touch7 = 0x300,
		TS_Touch8 = 0x311,
		TS_Touch9 = 0x312,
		TS_Touch10 = 0x313,
		TS_Touch11 = 0x314,
		TS_Touch12 = 0x315,
		TS_Touch13 = 0x316,
		TS_Touch14 = 0x317,
		TS_Touch15 = 0x318,

		TS_AnyTouch
	};

	enum SensorSemantic
	{
		SS_Latitude = 0x400,
		SS_Longitude,
		SS_Altitude,
		SS_LocationErrorRadius,
		SS_LocationAltitudeError,
		SS_Speed,
		SS_Accel,
		SS_AngularVelocity,
		SS_Tilt,
		SS_MagneticHeadingNorth,
		SS_OrientationQuat,
		SS_MagnetometerAccuracy,

		SS_AnySensing
	};

	struct AIR_CORE_API InputeActionDefine
	{
		InputeActionDefine(uint32_t a, uint32_t s)
			: action(static_cast<uint16_t>(a)), semantic(static_cast<uint16_t>(s))
		{

		}
		uint16_t action;
		uint16_t semantic;
	};

	typedef std::pair<uint16_t, InputActionParamPtr> InputAction;
	typedef std::vector<InputAction> InputActionsType;

	class AIR_CORE_API InputActionMap
	{
	public:
		void addAction(InputeActionDefine const & action_define);
		template <typename ForwardIterator>
		void addActions(ForwardIterator first, ForwardIterator last)
		{
			for (ForwardIterator iter = first; iter != last; ++iter)
			{
				this->addAction(*iter);
			}
		}

		void updateInputActions(InputActionsType& actions, uint16_t key, InputActionParamPtr const & param);

		bool hasAction(uint16_t key) const;
		uint16_t getAction(uint16_t key) const;
	private:
		boost::container::flat_map<uint16_t, uint16_t> mActionMap;

	};

	typedef boost::signals2::signal<void(InputEngine const & sender, InputAction const & action)> InputSignal;
	typedef std::shared_ptr<InputSignal> ActionHandlerT;
	typedef boost::container::flat_map<uint32_t, InputActionMap> ActionMapsT;


	class AIR_CORE_API InputEngine : boost::noncopyable
	{
	public:
		enum InputDeviceType
		{
			IDT_Keyboard,
			IDT_Mouse,
			IDT_Joystick,
			IDT_Touch,
			IDT_Sensor
		};
	public:	
		virtual ~InputEngine();
		void suspend();
		void resume();

		virtual std::wstring const & getName() const = 0;
		virtual void enumDevices() = 0;
		void update();

		float elapsedTime() const;

		void actionMap(InputActionMap const  actionMap, ActionHandlerT handler);

		size_t getNumDevice() const;
		InputDevicePtr getDevice(size_t index) const;

	private:
		virtual void doSuspend() = 0;
		virtual void doResume() = 0;

	protected:
		std::vector<InputDevicePtr> mDevices;
		std::vector<std::pair<InputActionMap, ActionHandlerT>> mActionHandlers;
		Timer mTimer;
		float mElapsedTime;
	};

	class AIR_CORE_API InputDevice : boost::noncopyable
	{
	public:
		virtual ~InputDevice();
		virtual std::wstring const & getName() const = 0;
		virtual InputEngine::InputDeviceType getType() const = 0;
		virtual void updateInputes() = 0;
		virtual InputActionsType updateActionMap(uint32_t id) = 0;

		virtual void actionMap(uint32_t id, InputActionMap const & actiionMap) = 0;
	protected:
		ActionMapsT mActionMaps;
	};

	class AIR_CORE_API InputKeyboard : public InputDevice
	{
	public:
		InputKeyboard();
		virtual ~InputKeyboard();

		virtual InputEngine::InputDeviceType getType() const override
		{
			return InputEngine::IDT_Keyboard;
		}

		size_t getNumKeys() const
		{
			return mKeys.size();
		}
		bool getKey(size_t n) const;
		bool const * getKeys() const;

		bool isKeyDown(size_t n) const;
		bool isKeyUp(size_t n) const;

		virtual InputActionsType updateActionMap(uint32_t id) override;

		virtual void actionMap(uint32_t id, InputActionMap const & actiionMap) override;

	protected:
		std::array<std::array<bool, 256>, 2> mKeys;
		bool mIndex;
		InputKeyboardActionParamPtr mActionParam;

	};

	class AIR_CORE_API InputMouse : public InputDevice
	{
	public:
		InputMouse();
		virtual ~InputMouse();
		virtual InputEngine::InputDeviceType getType() const override
		{
			return InputEngine::IDT_Mouse;
		}

		long getAbsX() const;
		long getAbsY() const;

		long getX() const;
		long getY() const;
		long getZ() const;
		bool isLeftButton() const;
		bool isRightButton() const;
		bool isMiddleButton() const;

		size_t getNumButtons() const;
		bool getButton(size_t n) const;
		bool isButtonDown(size_t n) const;
		bool isButtonUp(size_t n) const;

		virtual InputActionsType updateActionMap(uint32_t id) override;
		virtual void actionMap(uint32_t id, InputActionMap const & actiionMap) override;

	protected:
		int2 mAbsPos;
		int3 mOffset;
		uint32_t mNumButtons;
		std::array<std::array<bool, 8>, 2> mButtons;
		bool mIndex;
		uint16_t mShiftCtrlAlt;
		InputMouseActionParamPtr mActionParam;
	};

	struct AIR_CORE_API InputActionParam
	{
		virtual ~InputActionParam()
		{

		};
		InputEngine::InputDeviceType type;
	};


	struct AIR_CORE_API InputKeyboardActionParam : public InputActionParam
	{
		std::bitset<256> mButtonsState;
		std::bitset<256> mButtonsDown;
		std::bitset<256> mButtonsUp;
	};

	enum MouseButtons
	{
		MB_Left = 1UL << 0,
		MB_Right = 1UL << 1,
		MB_Middle = 1UL << 2,
		MB_Button0 = 1UL << 0,
		MB_Button1 = 1UL << 1,
		MB_Button2 = 1UL << 2,
		MB_Button3 = 1UL << 3,
		MB_Button4 = 1UL << 4,
		MB_Button5 = 1UL << 5,
		MB_Button6 = 1UL << 6,
		MB_Button7 = 1UL << 7,
		MB_Shift = 1UL << 8,
		MB_Ctrl = 1UL << 9,
		MB_Alt = 1UL << 10
	};

	struct AIR_CORE_API InputMouseActionParam : public InputActionParam
	{
		int2 mMoveVec;
		int32_t mWheelDelta;
		int2 mAbsCoord;
		uint16_t mButtonsState;
		uint16_t mButtonsDown;
		uint16_t mButtonsUp;
	};

}



#endif
