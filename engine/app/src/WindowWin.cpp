#include "Engine.h"

#ifdef AIR_PLATFORM_WINDOWS_DESKTOP

#include <basic/include/Math.hpp>

#include <app/include/Window.hpp>

#if (_WIN32_WINNT >= _WIN32_WINNT_WINBLUE)
#include <VersionHelpers.h>
#include <ShellScalingApi.h>
#endif

#include <windowsx.h>
#ifndef GET_KEYSTATE_WPARAM
#define GET_KEYSTATE_WPARAM(wParam) (LOWORD(wParam))
#endif // !GET_KEYSTATE_WPARAM

namespace Air
{
	LRESULT Window::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
#ifdef AIR_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4312)
#endif
		Window* win = reinterpret_cast<Window*>(::GetWindowLongPtrW(hWnd, GWLP_USERDATA));
#ifdef AIR_COMPILER_MSVC
#pragma warning(pop)
#endif
		if (win != nullptr)
		{
			return win->msgProc(hWnd, uMsg, wParam, lParam);
		}
		else
		{
			return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
#if (_WIN32_WINNT >= _WIN32_WINNT_WINBLUE)

	BOOL Window::EnumMonProc(HMONITOR mon, HDC dc_mon, RECT* rc_mon, LPARAM lparam)
	{

		HMODULE shcore = ::LoadLibraryEx(TEXT("SHCore.dll"), nullptr, 0);
		if (shcore)
		{
			typedef HRESULT(CALLBACK *GetDpiForMonitorFunc)(HMONITOR mon, MONITOR_DPI_TYPE dpi_type, UINT* dpi_x, UINT* dpi_y);
			GetDpiForMonitorFunc DynamicGetDpiForMonitor
				= reinterpret_cast<GetDpiForMonitorFunc>(::GetProcAddress(shcore, "GetDpiForMonitor"));
			if (DynamicGetDpiForMonitor)
			{
				UINT dpi_x, dpi_y;
				if (S_OK == DynamicGetDpiForMonitor(mon, MDT_DEFAULT, &dpi_x, &dpi_y))
				{
					Window* win = reinterpret_cast<Window*>(lparam);
					win->mDpiScale = std::max(win->mDpiScale, static_cast<float>(std::max(dpi_x, dpi_y)) / USER_DEFAULT_SCREEN_DPI);
				}
			}

			::FreeLibrary(shcore);
		}

		return TRUE;
	}
#endif

	Window::Window(std::string const & name, RenderSettings const & settings)
		:mActive(false), mReady(false), mClosed(false), mDpiScale(1), mWinRotation(WR_Identity), mHide(settings.hide_win)
	{
		this->detectsDPI();
		HINSTANCE hInst = ::GetModuleHandle(nullptr);
		mName = name;
		WNDCLASSEXW wc;
		wc.cbSize = sizeof(wc);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = wndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = sizeof(this);
		wc.hInstance = hInst;
		wc.hIcon = nullptr;
		wc.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
		wc.hbrBackground = static_cast<HBRUSH>(::GetStockObject(BLACK_BRUSH));
		wc.lpszMenuName = nullptr;
		wc.lpszClassName = mNameW.c_str();
		wc.hIconSm = nullptr;
		::RegisterClassExW(&wc);
		uint32_t style;
		if (settings.full_screen)
		{
			style = WS_POPUP;
		}
		else
		{
			style = WS_OVERLAPPEDWINDOW;
		}
		RECT rc = { 0, 0, settings.width, settings.hide_win };
		::AdjustWindowRect(&rc, style, false);
		mWnd = ::CreateWindowW(mNameW.c_str(), mNameW.c_str(), style, settings.left, settings.top, rc.right - rc.left, rc.bottom - rc.top, 0, 0, hInst, nullptr
			);

		mDefaultWndProc = ::DefWindowProc;
		mExternalWnd = false;
		::GetClientRect(mWnd, &rc);
		mLeft = rc.left;
		mTop = rc.top;
		mWidth = rc.right - rc.left;
		mHeight = rc.bottom = rc.top;

#ifdef AIR_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4244) // Pointer to LONG_PTR, possible loss of data
#endif
		::SetWindowLongPtrW(mWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
#ifdef AIR_COMPILER_MSVC
#pragma warning(pop)
#endif

		::ShowWindow(mWnd, mHide ? SW_HIDE : SW_SHOWNORMAL);
		::UpdateWindow(mWnd);

		mReady= true;
	}






}






#endif