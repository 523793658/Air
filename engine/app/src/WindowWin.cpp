#include "Context.h"

#ifdef AIR_PLATFORM_WINDOWS_DESKTOP

#include <basic/include/Math.hpp>
#include <Basic/include/Util.h>
#include <basic/include/Timer.hpp>
#include <app/include/Window.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"

#if (_WIN32_WINNT >= _WIN32_WINNT_WINBLUE)
#include <VersionHelpers.h>
#include <ShellScalingApi.h>
#endif
#include <ImageHlp.h>
#include <windows.h>
#include <windowsx.h>
#ifndef GET_KEYSTATE_WPARAM
#define GET_KEYSTATE_WPARAM(wParam) (LOWORD(wParam))
#endif // !GET_KEYSTATE_WPARAM

#pragma comment(lib, "dbghelp.lib")
#define BOOST_DATE_TIME_SOURCE

namespace Air
{
	LONG WINAPI UnhandledExceptionFilter2(struct _EXCEPTION_POINTERS* ExceptionInfo)
	{
		//std::wstring strDumpFile = boost::posix_time::to_iso_wstring(boost::posix_time::second_clock::local_time());
		std::wstring strDumpFile = L"dump.dump";
		HANDLE hFile = CreateFile(strDumpFile.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			MINIDUMP_EXCEPTION_INFORMATION ExInfo;
			ExInfo.ThreadId = ::GetCurrentThreadId();
			ExInfo.ExceptionPointers = ExceptionInfo;
			ExInfo.ClientPointers = NULL;
			bool bOk = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpWithFullMemory, &ExInfo, NULL, NULL);
			CloseHandle(hFile);
		}
		return EXCEPTION_EXECUTE_HANDLER;
	}

	void initPlatform()
	{
		SetUnhandledExceptionFilter(UnhandledExceptionFilter2);
	}

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
		mName = convert(mName, name);
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
		wc.lpszClassName = mName.c_str();
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
		RECT rc = { 0, 0, settings.width, settings.height };
		::AdjustWindowRect(&rc, style, false);
		mWnd = ::CreateWindowW(mName.c_str(), mName.c_str(), style, settings.left, settings.top, rc.right - rc.left, rc.bottom - rc.top, 0, 0, hInst, nullptr
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


	Window::Window(std::string const & name, RenderSettings const & settings, void* native_wnd)
		:mActive(false), mReady(false), mClosed(false), mDpiScale(1), mWinRotation(WR_Identity), mHide(settings.hide_win)
	{
		this->detectsDPI();
		convert(mName, name);
		mWnd = static_cast<HWND>(native_wnd);
		mDefaultWndProc = reinterpret_cast<WNDPROC>(::GetWindowLongPtrW(mWnd, GWLP_WNDPROC));
		::SetWindowLongPtrW(mWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wndProc));
		mExternalWnd = true;
		RECT rc;
		::GetClientRect(mWnd, &rc);
		mLeft = rc.left;
		mTop = rc.top;
		mWidth = rc.right - rc.left;
		mHeight = rc.bottom - rc.top;

#ifdef AIR_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4244)
#endif

		::SetWindowLongPtrW(mWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
#ifdef AIR_COMPILER_MSVC
#pragma warning(pop)
#endif
		::UpdateWindow(mWnd);
		mReady = true;

	}

	Window::~Window()
	{
		if (mWnd != nullptr)
		{
#ifdef AIR_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4244)
#endif
			::SetWindowLongPtrW(mWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(nullptr));
#ifdef AIR_COMPILER_MSVC
#pragma warning(pop)
#endif
			if (!mExternalWnd)
			{
				::DestroyWindow(mWnd);
			}
			mWnd = nullptr;
		}
	}

	void Window::recreate()
	{
		if (!mExternalWnd)
		{
			mReady = false;
			this->detectsDPI();
			HINSTANCE hInstance = ::GetModuleHandle(nullptr);
			uint32_t style = static_cast<uint32_t>(::GetWindowLongPtrW(mWnd, GWL_STYLE));
			RECT rc = { 0, 0, static_cast<LONG>(mWidth), static_cast<LONG>(mHeight) };
			::AdjustWindowRect(&rc, style, false);
			::DestroyWindow(mWnd);

			mWnd = ::CreateWindowW(mName.c_str(), mName.c_str(), style, mLeft, mTop, rc.right - rc.left, rc.bottom - rc.top, 0, 0, hInstance, nullptr);
			::GetClientRect(mWnd, &rc);
			mLeft = rc.left;
			mTop = rc.top;
			mWidth = rc.right - rc.left;
			mHeight = rc.bottom - rc.top;
#ifdef AIR_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4244)
#endif

			::SetWindowLongPtrW(mWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
#ifdef AIR_COMPILER_MSVC
#pragma warning(pop)
#endif
			::ShowWindow(mWnd, mHide ? SW_HIDE : SW_SHOWNORMAL);
			::UpdateWindow(mWnd);
			mReady = true;
		}
	}


	LRESULT Window::msgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_ACTIVATE:
			mActive = (WA_INACTIVE != LOWORD(wParam));
			this->onActive();
			break;
		case WM_ERASEBKGND:
			return 1;
		case WM_PAINT:
			this->onPaint();
			break;
		case WM_ENTERSIZEMOVE:
			mReady = false;
			this->onEnterSizeMove();
			break;
		case WM_EXITSIZEMOVE:
			this->onExitSizeMove();
			mReady = true;
			break;
		case WM_SIZE:
		{
			RECT rc;
			::GetClientRect(mWnd, &rc);
			mLeft = rc.left;
			mTop = rc.top;
			mWidth = rc.right - rc.left;
			mHeight = rc.bottom - rc.top;
			if ((SIZE_MAXHIDE == wParam) || (SIZE_MINIMIZED == wParam))
			{
				mActive = false;
				this->onSize()(*this, false);
			}
			else
			{
				mActive = true;
				this->onSize()(*this, true);
			}
		}
			break;
		case WM_GETMINMAXINFO:
			reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.x = 100;
			reinterpret_cast<MINMAXINFO*>(lParam)->ptMinTrackSize.y = 100;
			break;
		case WM_SETCURSOR:
			this->onSetCursor()(*this);
			break;
		case WM_CHAR:
			this->onChar()(*this, static_cast<wchar_t>(lParam));
			break;
		case WM_INPUT:
			this->onRawInput()(*this, reinterpret_cast<HRAWINPUT>(lParam));
			break;
#if(_WIN32_WINNT >= _WIN32_WINNT_WIN8)
// 		case WM_POINTERDOWN:
// 		{
// 			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
// 			::ScreenToClient(this->getHWnd(), &pt);
// 			this->onPointerDown()(*this, int2(pt.x, pt.y), GET_POINTERID_WPARAM(wParam));
// 		}
		break;

		case WM_POINTERUP:
// 		{
// 			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
// 			::ScreenToClient(this->getHWnd(), &pt);
// 			this->onPointerUp()(*this, int2(pt.x, pt.y), GET_POINTERID_WPARAM(wParam));
// 		}
		case WM_POINTERUPDATE:
// 		{
// 			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
// 			::ScreenToClient(this->getHWnd(), &pt);
// 			this->onPointerUpdate()(*this, int2(pt.x, pt.y), GET_POINTERID_WPARAM(wParam),
// 				IS_POINTER_INCONTACT_WPARAM(wParam));
// 		}
		break;

		case WM_POINTERWHEEL:
// 		{
// 			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
// 			::ScreenToClient(this->getHWnd(), &pt);
// 			this->onPointerWheel()(*this, int2(pt.x, pt.y), GET_POINTERID_WPARAM(wParam),
// 				GET_WHEEL_DELTA_WPARAM(wParam));
// 		}
		break;
#if (_WIN32_WINNT >= _WIN32_WINNT_WINBLUE)
		case WM_DPICHANGED:
			mDpiScale = static_cast<float>(HIWORD(wParam)) / USER_DEFAULT_SCREEN_DPI;
			break;
#endif

#elif (_WIN32_WINNT >= _WIN32_WINNT_WIN7)
		case WM_TOUCH:
			this->OnTouch()(*this, reinterpret_cast<HTOUCHINPUT>(lParam), LOWORD(wParam));
			break;
#endif
		case WM_CLOSE:
			this->onClose()(*this);
			mActive = false;
			mReady = false;
			mClosed = true;
			::PostQuitMessage(0);
			return 0;
		}
		return mDefaultWndProc(hWnd, uMsg, wParam, lParam);
	}


	void Window::detectsDPI()
	{
#if (_WIN32_WINNT >= _WIN32_WINNT_WINBLUE)
		HMODULE shcore = ::LoadLibraryEx(TEXT("SHCore.dll"), nullptr, 0);
		if (shcore)
		{
			typedef HRESULT(WINAPI *SetProcessDpiAwarenessFunc)(PROCESS_DPI_AWARENESS value);
			SetProcessDpiAwarenessFunc DynamicSetProcessDpiAwareness
				= reinterpret_cast<SetProcessDpiAwarenessFunc>(::GetProcAddress(shcore, "SetProcessDpiAwareness"));

			DynamicSetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

			::FreeLibrary(shcore);
		}

		typedef NTSTATUS(WINAPI *RtlGetVersionFunc)(OSVERSIONINFOEXW* pVersionInformation);
		HMODULE ntdll = ::GetModuleHandleW(L"ntdll.dll");
		AIR_ASSUME(ntdll != nullptr);
		RtlGetVersionFunc DynamicRtlGetVersion = reinterpret_cast<RtlGetVersionFunc>(::GetProcAddress(ntdll, "RtlGetVersion"));
		if (DynamicRtlGetVersion)
		{
			OSVERSIONINFOEXW os_ver_info;
			os_ver_info.dwOSVersionInfoSize = sizeof(os_ver_info);
			DynamicRtlGetVersion(&os_ver_info);

			if ((os_ver_info.dwMajorVersion > 6) || ((6 == os_ver_info.dwMajorVersion) && (os_ver_info.dwMinorVersion >= 3)))
			{
				HDC desktop_dc = ::GetDC(nullptr);
				::EnumDisplayMonitors(desktop_dc, nullptr, EnumMonProc, reinterpret_cast<LPARAM>(this));
				::ReleaseDC(nullptr, desktop_dc);
			}
		}
#endif
	}

}






#endif