#ifndef _Air_Window_H_
#define _Air_Window_H_
#pragma once
#include "core/include/PreDeclare.h"

#include "rendersystem/include/RenderSettings.hpp"

#if defined(AIR_COMPILER_MSVC)
#pragma warning(push)
#pragma warning(disable: 4512)
#pragma warning(disable: 4915)
#elif defined(AIR_COMPILER_GCC)
#endif

#if defined(AIR_COMPILER_MSVC)
#pragma warning(pop)
#elif defined(AIR_COMPILER_GCC)

#endif

#if defined AIR_PLATFORM_WINDOWS_DESKTOP

#include <windows.h>
#endif

#include <string.h>

namespace Air
{
	class AIR_CORE_API Window
	{
	public:
		enum WindowRotation
		{
			WR_Unspecified,
			WR_Identity,
			WR_Rotate90,
			WR_Rotate180,
			WR_Rotate270
		};

	public:
		Window(std::string const & name, RenderSettings const & settings);
		Window(std::string const & name, RenderSettings const & settings, void* native_wnd);
		~Window();
#if defined AIR_PLATFORM_WINDOWS_DESKTOP
		void recreate();
		HWND getHWnd() const
		{

		}
#endif
#if defined AIR_PLATFORM_WINDOWS
#if defined AIR_PLATFORM_WINDOWS_DESKTOP
	private:
		static LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#if (_WIN32_WINNT >= _WIN32_WINNT_WINBLUE)
		static BOOL CALLBACK EnumMonProc(HMONITOR mon, HDC dc_mon, RECT* rc_mon, LPARAM lparam);
#endif
		LRESULT msgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#else
		void DetectsOrientation();
#endif
		void detectsDPI();

#endif


	private:
		int32_t mLeft;
		int32_t mTop;
		int32_t mWidth;
		int32_t mHeight;

		bool mActive;
		bool mReady;
		bool mClosed;

		float mDpiScale;
		WindowRotation mWinRotation;
#if defined AIR_PLATFORM_WINDOWS
		bool mHide;
		bool mExternalWnd;
		std::wstring mNameW;
		std::string mName;
#if defined AIR_PLATFORM_WINDOWS_DESKTOP
		HWND mWnd;
		WNDPROC mDefaultWndProc;
#endif
#endif
	};
}




#endif