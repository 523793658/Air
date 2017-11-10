#include "Context.h"
#include "Engine.h"
#include "basic/include/Util.h"
#include "basic/include/ErrorHanding.hpp"
#include "app/include/App3D.hpp"
#include "app/include/Window.hpp"
#include <system_error>
#include "MInput.hpp"


namespace Air
{
	MsgInputEngine::MsgInputEngine()
	{
		mModHid = ::LoadLibraryEx(TEXT("hid.dll"), nullptr, 0);
		if (nullptr == mModHid)
		{
			::MessageBoxW(nullptr, L"Can't load hid.dll", L"Error", MB_OK);
		}
		if (mModHid != nullptr)
		{
			DynamicHidP_GetCaps_ = reinterpret_cast<HidP_GetCapsFunc>(::GetProcAddress(mModHid, "HidP_GetCaps"));

			DynamicHidP_GetButtonCaps_ = reinterpret_cast<HidP_GetButtonCapsFunc>(::GetProcAddress(mModHid, "HidP_GetButtonCaps"));
			DynamicHidP_GetValueCaps_ = reinterpret_cast<HidP_GetValueCapsFunc>(::GetProcAddress(mModHid, "HidP_GetValueCaps"));
			DynamicHidP_GetUsages_ = reinterpret_cast<HidP_GetUsagesFunc>(::GetProcAddress(mModHid, "HidP_GetUsages"));
			DynamicHidP_GetUsageValue_ = reinterpret_cast<HidP_GetUsageValueFunc>(::GetProcAddress(mModHid, "HidP_GetUsageValue"));
		}
	}

	MsgInputEngine::~MsgInputEngine()
	{
		mOnRawInput.disconnect();
		mOnPointerDown.disconnect();
		mOnPointerUp.disconnect();
		mOnPointerUpdate.disconnect();
		mOnPointerWheel.disconnect();
		mDevices.clear();
		::FreeLibrary(mModHid);
	}

	void MsgInputEngine::doSuspend()
	{

	}
	void MsgInputEngine::doResume()
	{

	}
	std::wstring const &MsgInputEngine::getName() const
	{
		static std::wstring const name(L"Message-based Input Engine");
		return name;
	}

	void MsgInputEngine::enumDevices()
	{
		WindowPtr const & main_wnd = Engine::getInstance().getAppInstance().getMainWnd();
		HWND hwnd = main_wnd->getHWnd();
		UINT devices = 0;
		if (::GetRawInputDeviceList(nullptr, &devices, sizeof(RAWINPUTDEVICELIST)) != 0)
		{
			TERRC(std::errc::function_not_supported);
		}
		std::vector<RAWINPUTDEVICELIST> raw_input_devices(devices);
		::GetRawInputDeviceList(&raw_input_devices[0], &devices, sizeof(raw_input_devices[0]));
		RAWINPUTDEVICE rid;
		rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
		rid.hwndTarget = hwnd;
		std::vector<RAWINPUTDEVICE> rids;
		for (size_t i = 0; i < raw_input_devices.size(); ++i)
		{
			InputDevicePtr device;
			switch (raw_input_devices[i].dwType)
			{
			case RIM_TYPEKEYBOARD:
				device = MakeSharedPtr<MsgInputKeyboard>();
				rid.usUsage = HID_USAGE_GENERIC_KEYBOARD;
				rid.dwFlags = 0;
				rids.push_back(rid);
				break;
			case RIM_TYPEMOUSE:
				device = MakeSharedPtr<MsgInputMouse>(hwnd, raw_input_devices[i].hDevice);
				rid.usUsage = HID_USAGE_GENERIC_MOUSE;
				rid.dwFlags = 0;
				rids.push_back(rid);
				break;
			default:
				break;
			}
			if (device)
			{
				mDevices.push_back(device);
			}
		}
		if (::RegisterRawInputDevices(&rids[0], static_cast<UINT>(rids.size()), sizeof(rids[0])))
		{
			mOnRawInput = main_wnd->onRawInput().connect(bind(&MsgInputEngine::onRawInput, this, std::placeholders::_1, std::placeholders::_2));
		}

		mOnPointerDown = main_wnd->onPointerDown().connect(std::bind(&MsgInputEngine::onPointerDown, this, std::placeholders::_2, std::placeholders::_3));

		mOnPointerUp = main_wnd->onPointerUp().connect(std::bind(&MsgInputEngine::onPointerUp, this, std::placeholders::_2, std::placeholders::_3));

		mOnPointerUpdate = main_wnd->onPointerUpdate().connect(std::bind(&MsgInputEngine::onPointerUpdate, this, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	}



	void MsgInputEngine::onRawInput(Window const & wnd, HRAWINPUT ri)
	{
		if (wnd.getHWnd() == ::GetForegroundWindow())
		{
			UINT size = 0;
			if (0 == ::GetRawInputData(ri, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)))
			{
				std::vector<uint8_t> data(size);
				::GetRawInputData(ri, RID_INPUT, &data[0], &size, sizeof(RAWINPUTHEADER));
				RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(&data[0]);
				for (auto const & device : mDevices)
				{
					switch (raw->header.dwType)
					{
					case RIM_TYPEKEYBOARD:
						if (IDT_Keyboard == device->getType())
						{
							checked_pointer_cast<MsgInputKeyboard>(device)->onRawInput(*raw);
						}
						break;
					case RIM_TYPEMOUSE:
						if (IDT_Mouse == device->getType())
						{
							checked_pointer_cast<MsgInputMouse>(device)->onRawInput(*raw);
						}
						break;
					default:
						break;
					}
				}
			}
		}
	}

	void MsgInputEngine::onPointerDown(int2 const & pt, uint32_t n)
	{
		
	}

	void MsgInputEngine::onPointerUp(int2 const & pt, uint32_t id)
	{

	}

	void MsgInputEngine::onPointerUpdate(int2 const & pt, uint32_t id, bool down)
	{

	}

	void MsgInputEngine::onPointerWheel(int2 const & pt, uint32_t id, int32_t wheel_delta)
	{

	}

	NTSTATUS MsgInputEngine::HidP_GetCaps(PHIDP_PREPARSED_DATA PreparsedData, PHIDP_CAPS Capabilities) const
	{
		return DynamicHidP_GetCaps_(PreparsedData, Capabilities);
	}

	NTSTATUS MsgInputEngine::HidP_GetButtonCaps(HIDP_REPORT_TYPE ReportType, PHIDP_BUTTON_CAPS ButtonCaps,
		PUSHORT ButtonCapsLength, PHIDP_PREPARSED_DATA PreparsedData) const
	{
		return DynamicHidP_GetButtonCaps_(ReportType, ButtonCaps, ButtonCapsLength, PreparsedData);
	}

	NTSTATUS MsgInputEngine::HidP_GetValueCaps(HIDP_REPORT_TYPE ReportType, PHIDP_VALUE_CAPS ValueCaps,
		PUSHORT ValueCapsLength, PHIDP_PREPARSED_DATA PreparsedData) const
	{
		return DynamicHidP_GetValueCaps_(ReportType, ValueCaps, ValueCapsLength, PreparsedData);
	}

	NTSTATUS MsgInputEngine::HidP_GetUsages(HIDP_REPORT_TYPE ReportType, USAGE UsagePage,
		USHORT LinkCollection, PUSAGE UsageList, PULONG UsageLength, PHIDP_PREPARSED_DATA PreparsedData,
		PCHAR Report, ULONG ReportLength) const
	{
		return DynamicHidP_GetUsages_(ReportType, UsagePage, LinkCollection, UsageList, UsageLength, PreparsedData,
			Report, ReportLength);
	}

	NTSTATUS MsgInputEngine::HidP_GetUsageValue(HIDP_REPORT_TYPE ReportType, USAGE UsagePage,
		USHORT LinkCollection, USAGE Usage, PULONG UsageValue, PHIDP_PREPARSED_DATA PreparsedData,
		PCHAR Report, ULONG ReportLength) const
	{
		return DynamicHidP_GetUsageValue_(ReportType, UsagePage, LinkCollection, Usage, UsageValue, PreparsedData,
			Report, ReportLength);
	}
}

namespace
{
	using namespace Air;
	static int32_t const VK_MAPPING[] =
	{
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		KS_BackSpace,
		KS_Tab,
		-1,
		-1,
		-1,
		KS_Enter,
		-1,
		-1,
		KS_LeftShift,
		KS_LeftCtrl,
		KS_LeftAlt,
		KS_Pause,
		KS_CapsLock,
		KS_Kana,
		-1,
		-1,
		-1,
		-1,
		-1,
		KS_Escape,
		KS_Convert,
		KS_NoConvert,
		-1,
		-1,
		KS_Space,
		KS_PageUp,
		KS_PageDown,
		KS_End,
		KS_Home,
		KS_LeftArrow,
		KS_UpArrow,
		KS_RightArrow,
		KS_DownArrow,
		-1,
		-1,
		-1,
		-1,
		KS_Insert,
		KS_Delete,
		-1,
		KS_0,
		KS_1,
		KS_2,
		KS_3,
		KS_4,
		KS_5,
		KS_6,
		KS_7,
		KS_8,
		KS_9,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		KS_A,
		KS_B,
		KS_C,
		KS_D,
		KS_E,
		KS_F,
		KS_G,
		KS_H,
		KS_I,
		KS_J,
		KS_K,
		KS_L,
		KS_M,
		KS_N,
		KS_O,
		KS_P,
		KS_Q,
		KS_R,
		KS_S,
		KS_T,
		KS_U,
		KS_V,
		KS_W,
		KS_X,
		KS_Y,
		KS_Z,
		KS_LeftWin,
		KS_RightWin,
		KS_Apps,
		-1,
		KS_Sleep,
		KS_NumPad0,
		KS_NumPad1,
		KS_NumPad2,
		KS_NumPad3,
		KS_NumPad4,
		KS_NumPad5,
		KS_NumPad6,
		KS_NumPad7,
		KS_NumPad8,
		KS_NumPad9,
		KS_NumPadStar,
		KS_Equals,
		KS_BackSlash,
		KS_Minus,
		-1,
		KS_Slash,
		KS_F1,
		KS_F2,
		KS_F3,
		KS_F4,
		KS_F5,
		KS_F6,
		KS_F7,
		KS_F8,
		KS_F9,
		KS_F10,
		KS_F11,
		KS_F12,
		KS_F13,
		KS_F14,
		KS_F15,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		KS_NumLock,
		KS_ScrollLock,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		KS_LeftShift,
		KS_RightShift,
		KS_LeftCtrl,
		KS_RightCtrl,
		KS_LeftAlt,
		KS_RightAlt,
		KS_WebBack,
		KS_WebForward,
		KS_WebRefresh,
		KS_WebStop,
		KS_WebSearch,
		KS_WebFavorites,
		KS_WebHome,
		KS_Mute,
		KS_VolumeDown,
		KS_VolumeUp,
		KS_NextTrack,
		KS_PrevTrack,
		KS_MediaStop,
		KS_PlayPause,
		KS_Mail,
		KS_MediaSelect,
		-1,
		-1,
		-1,
		-1,
		KS_Comma,
		KS_NumPadPlus,
		-1,
		KS_NumPadMinus,
		KS_NumPadPeriod,
		KS_Slash,
		KS_Grave,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		KS_LeftBracket,
		KS_BackSlash,
		KS_RightBracket,
		KS_Grave,
		-1,
		-1,
		-1,
		KS_OEM_102,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1,
		-1
	};




}

namespace Air
{
	MsgInputKeyboard::MsgInputKeyboard()
	{
		mKeysState.fill(false);
	}
	std::wstring const & MsgInputKeyboard::getName() const
	{
		static std::wstring const name(L"MsgInput Keyboard");
		return name;
	}

	void MsgInputKeyboard::onRawInput(RAWINPUT const & ri)
	{
		BOOST_ASSERT(RIM_TYPEKEYBOARD == ri.header.dwType);

		int32_t ks = VK_MAPPING[ri.data.keyboard.VKey];
		if (ks >= 0)
		{
			mKeysState[ks] = (RI_KEY_MAKE == (ri.data.keyboard.Flags & 1UL));
		}
	}


	void MsgInputKeyboard::updateInputs()
	{
		mIndex = !mIndex;
		mKeys[mIndex] = mKeysState;
	}

	MsgInputMouse::MsgInputMouse(HWND hwnd, HANDLE device)
		:mHwnd(hwnd), mDeviceId(0xFFFFFFFF), mLastAbsState(0x7FFFFFFF, 0x7FFFFFFF), mOffsetState(0, 0, 0)
	{
		mButtonsState.fill(false);
		UINT size = 0;
		if (0 == ::GetRawInputDeviceInfo(device, RIDI_DEVICEINFO, nullptr, &size))
		{
			std::vector<uint8_t> buf(size);
			::GetRawInputDeviceInfo(device, RIDI_DEVICEINFO, &buf[0], &size);

			RID_DEVICE_INFO* info = reinterpret_cast<RID_DEVICE_INFO*>(&buf[0]);
			mDeviceId = info->mouse.dwId;
			mNumButtons = std::min(static_cast<uint32_t>(mButtons[0].size()),
				static_cast<uint32_t>(info->mouse.dwNumberOfButtons));
		}
	}

	std::wstring const & MsgInputMouse::getName() const
	{
		static std::wstring const name(L"MsgInput Mouse");
		return name;
	}

	void MsgInputMouse::onRawInput(RAWINPUT const & ri)
	{
		BOOST_ASSERT(RIM_TYPEMOUSE == ri.header.dwType);

		UINT size = 0;
		if (0 == ::GetRawInputDeviceInfo(ri.header.hDevice, RIDI_DEVICEINFO, nullptr, &size))
		{
			std::vector<uint8_t> buf(size);
			::GetRawInputDeviceInfo(ri.header.hDevice, RIDI_DEVICEINFO, &buf[0], &size);

			RID_DEVICE_INFO* info = reinterpret_cast<RID_DEVICE_INFO*>(&buf[0]);
			if (mDeviceId != info->mouse.dwId)
			{
				return;
			}
		}

		for (uint32_t i = 0; i < mNumButtons; ++i)
		{
			if (ri.data.mouse.usButtonFlags & (1UL << (i * 2 + 0)))
			{
				mButtonsState[i] = true;
			}
			if (ri.data.mouse.usButtonFlags & (1UL << (i * 2 + 1)))
			{
				mButtonsState[i] = false;
			}
		}

		if ((ri.data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE) == MOUSE_MOVE_ABSOLUTE)
		{
			bool const virtual_desktop = ((ri.data.mouse.usFlags & MOUSE_VIRTUAL_DESKTOP) == MOUSE_VIRTUAL_DESKTOP);
			int const width = ::GetSystemMetrics(virtual_desktop ? SM_CXVIRTUALSCREEN : SM_CXSCREEN);
			int const height = ::GetSystemMetrics(virtual_desktop ? SM_CYVIRTUALSCREEN : SM_CYSCREEN);
			int2 const new_point(static_cast<int>((ri.data.mouse.lLastX / 65535.0f) * width),
				static_cast<int>((ri.data.mouse.lLastY / 65535.0f) * height));
			if (mLastAbsState == int2(0x7FFFFFFF, 0x7FFFFFFF))
			{
				mLastAbsState = new_point;
			}

			mOffsetState.x() += new_point.x() - mLastAbsState.x();
			mOffsetState.y() += new_point.y() - mLastAbsState.y();
			mLastAbsState = new_point;
		}
		else
		{
			mOffsetState.x() += ri.data.mouse.lLastX;
			mOffsetState.y() += ri.data.mouse.lLastY;
		}
		if (ri.data.mouse.usButtonFlags & RI_MOUSE_WHEEL)
		{
			mOffsetState.z() += static_cast<short>(ri.data.mouse.usButtonData);
		}
	}

	void MsgInputMouse::updateInputs()
	{
		POINT pt;
		::GetCursorPos(&pt);
		::ScreenToClient(mHwnd, &pt);
		mAbsPos = int2(pt.x, pt.y);
		mOffset = mOffsetState;

		mIndex = !mIndex;
		mButtons[mIndex] = mButtonsState;

		mShiftCtrlAlt = ((::GetKeyState(VK_SHIFT) & 0x80) ? MB_Shift : 0)
			| ((::GetKeyState(VK_CONTROL) & 0x80) ? MB_Ctrl : 0)
			| ((::GetKeyState(VK_MENU) & 0x80) ? MB_Alt : 0);

		mOffsetState = int3(0, 0, 0);
	}
}