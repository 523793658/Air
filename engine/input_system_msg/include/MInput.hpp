#ifndef _MInput_H_
#define _MInput_H_
#pragma once

#if defined AIR_PLATFORM_WINDOWS_DESKTOP
#define INITGUID
#include <windows.h>
#if defined AIR_HAVE_LIBOVR
#include <OVR.h>
#endif
#if (_WIN32_WINNT >= _WIN32_WINNT_WINBLUE)
#include <hidsdi.h>
#endif
#if (defined AIR_PLATFORM_WINDOWS_DESKTOP)
#if (_WIN32_WINNT < _WIN32_WINNT_WIN10)
#include <locationapi.h>
#endif
#include <SensorsApi.h>
#include <sensors.h>
#endif
#endif

#include "input_system/include/Input.hpp"

namespace Air
{
	class MsgInputEngine : public InputEngine
	{
	public:
		MsgInputEngine();
		~MsgInputEngine();

		std::wstring const & getName() const;
		void enumDevices();

		NTSTATUS HidP_GetCaps(PHIDP_PREPARSED_DATA PreparsedData, PHIDP_CAPS Capabilities) const;
		NTSTATUS HidP_GetButtonCaps(HIDP_REPORT_TYPE ReportType, PHIDP_BUTTON_CAPS ButtonCaps,
			PUSHORT ButtonCapsLength, PHIDP_PREPARSED_DATA PreparsedData) const;
		NTSTATUS HidP_GetValueCaps(HIDP_REPORT_TYPE ReportType, PHIDP_VALUE_CAPS ValueCaps,
			PUSHORT ValueCapsLength, PHIDP_PREPARSED_DATA PreparsedData) const;
		NTSTATUS HidP_GetUsages(HIDP_REPORT_TYPE ReportType, USAGE UsagePage,
			USHORT LinkCollection, PUSAGE UsageList, PULONG UsageLength, PHIDP_PREPARSED_DATA PreparsedData,
			PCHAR Report, ULONG ReportLength) const;
		NTSTATUS HidP_GetUsageValue(HIDP_REPORT_TYPE ReportType, USAGE UsagePage,
			USHORT LinkCollection, USAGE Usage, PULONG UsageValue, PHIDP_PREPARSED_DATA PreparsedData,
			PCHAR Report, ULONG ReportLength) const;


	private:
		virtual void doSuspend() override;
		virtual void doResume() override;

		void onRawInput(Window const & wnd, HRAWINPUT ri);
	private:
		boost::signals2::connection mOnRawInput;

		boost::signals2::connection mOnPointerDown;
		boost::signals2::connection mOnPointerUp;
		boost::signals2::connection mOnPointerUpdate;
		boost::signals2::connection mOnPointerWheel;

		HMODULE mModHid;

		typedef NTSTATUS(WINAPI *HidP_GetCapsFunc)(PHIDP_PREPARSED_DATA PreparsedData, PHIDP_CAPS Capabilities);

		typedef NTSTATUS(WINAPI *HidP_GetButtonCapsFunc)(HIDP_REPORT_TYPE reportType, PHIDP_BUTTON_CAPS button_caps, PUSHORT ButtonCapsLength, PHIDP_PREPARSED_DATA preparsedData);

		typedef NTSTATUS(WINAPI *HidP_GetValueCapsFunc)(HIDP_REPORT_TYPE reportType, PHIDP_VALUE_CAPS valueCaps, PUSHORT valueCapsLength, PHIDP_PREPARSED_DATA preparedData);

		typedef NTSTATUS(WINAPI* HidP_GetUsagesFunc)(HIDP_REPORT_TYPE reportType, USAGE UsagePage, USHORT LinkCollection, PUSAGE UsageList, PULONG UsageLength, PHIDP_PREPARSED_DATA preparsedData, PCHAR report, ULONG reportLength);


		typedef NTSTATUS(WINAPI *HidP_GetUsageValueFunc)(HIDP_REPORT_TYPE ReportType, USAGE UsagePage,
			USHORT LinkCollection, USAGE Usage, PULONG UsageValue, PHIDP_PREPARSED_DATA PreparsedData,
			PCHAR Report, ULONG ReportLength);
		HidP_GetCapsFunc DynamicHidP_GetCaps_;
		HidP_GetButtonCapsFunc DynamicHidP_GetButtonCaps_;
		HidP_GetValueCapsFunc DynamicHidP_GetValueCaps_;
		HidP_GetUsagesFunc DynamicHidP_GetUsages_;
		HidP_GetUsageValueFunc DynamicHidP_GetUsageValue_;


		void onPointerDown(int2 const & pt, uint32_t n);
		void onPointerUp(int2 const & pt, uint32_t id);
		void onPointerUpdate(int2 const & pt, uint32_t id, bool down);
		void onPointerWheel(int2 const & pt, uint32_t id, int32_t wheel_delta);

	};

	class MsgInputKeyboard : public InputKeyboard
	{
	public:
		MsgInputKeyboard();
		virtual std::wstring const & getName() const override;
		
#if defined AIR_PLATFORM_WINDOWS_DESKTOP
		void onRawInput(RAWINPUT const & ri);
#elif defined(AIR_PLATFORM_ANDROID) || defined(AIR_PLATFORM_DARWIN)
		void onKeyDown(uint32_t key);
		void onKeyUp(uint32_t key);
#endif
	private:
		virtual void updateInputs() override;
	private:
		std::array<bool, 256> mKeysState;
	};

	class MsgInputMouse : public InputMouse
	{
	public:
#if defined AIR_PLATFORM_WINDOWS_DESKTOP
		MsgInputMouse(HWND hwnd, HANDLE device);
#endif
		virtual std::wstring const & getName() const override;
		void onRawInput(RAWINPUT const & ri);

	private:
		virtual void updateInputs() override;
	private:
		HWND mHwnd;
		uint32_t mDeviceId;
		uint2 mLastAbsState;
		int3 mOffsetState;
		std::array<bool, 8> mButtonsState;
	};

}

#endif
