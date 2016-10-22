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

#include <boost/signals2.hpp>

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
			return mWnd;
		}

		int32_t getLeft() const
		{
			return mLeft;
		}

		int32_t getTop() const
		{
			return mTop;
		}

		uint32_t getWidth() const
		{
			return mWidth;
		}
		uint32_t getHeight() const
		{
			return mHeight;
		}

		bool getActive() const
		{
			return mActive;
		}

		void setActive(bool active)
		{
			mActive = active;
		}
		bool getReady() const
		{
			return mReady;
		}

		void setReady(bool ready)
		{
			mReady = ready;
		}

		bool getClosed() const
		{
			return mClosed;
		}

		void setClosed(bool closed)
		{
			mClosed = closed;
		}

		float getDPIScale() const
		{
			return mDpiScale;
		}

		WindowRotation getRotation() const
		{
			return mWinRotation;
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

	public:
		typedef boost::signals2::signal<void(Window const & wnd, bool active)> ActiveEvent;
		typedef boost::signals2::signal<void(Window const & wnd)> PaintEvent;
		typedef boost::signals2::signal<void(Window const & wnd)> EnterSizeMoveEvent;
		typedef boost::signals2::signal<void(Window const & wnd)> ExitSizeMoveEvent;
		typedef boost::signals2::signal<void(Window const & wnd, bool active)> SizeEvent;
		typedef boost::signals2::signal<void(Window const & wnd)> SetCursorEvent;
		typedef boost::signals2::signal<void(Window const & wnd, wchar_t ch)> CharEvent;
#if defined AIR_PLATFORM_WINDOWS_DESKTOP
		typedef boost::signals2::signal<void(Window const & wnd, HRAWINPUT ri)> RawInputEvent;
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN7)
		typedef boost::signals2::signal<void(Window const & wnd, HTOUCHINPUT hti, uint32_t num_inputs)> TouchEvent;
#endif
#endif
		typedef boost::signals2::signal<void(Window const & wnd, int2 const & pt, uint32_t id)> PointerDownEvent;
		typedef boost::signals2::signal<void(Window const & wnd, int2 const & pt, uint32_t id)> PointerUpEvent;
		typedef boost::signals2::signal<void(Window const & wnd, int2 const & pt, uint32_t id, bool down)> PointerUpdateEvent;
		typedef boost::signals2::signal<void(Window const & wnd, int2 const & pt, uint32_t id, int32_t wheel_delta)> PointerWheelEvent;
		typedef boost::signals2::signal<void(Window const & wnd)> CloseEvent;

		ActiveEvent& onActive()
		{
			return mActiveEvent;
		}
		PaintEvent& onPaint()
		{
			return mPaintEvent;
		}
		EnterSizeMoveEvent& onEnterSizeMove()
		{
			return mEnterSizeMoveEvent;
		}
		ExitSizeMoveEvent& onExitSizeMove()
		{
			return mExitSizeMoveEvent;
		}

		SizeEvent& onSize()
		{
			return mSizeEvent;
		}
		SetCursorEvent& onSetCursor()
		{
			return mSetCursorEvent;
		}
		CharEvent& onChar()
		{
			return mCharEvent;
		}
#ifdef AIR_PLATFORM_WINDOWS_DESKTOP
		RawInputEvent& onRawInput()
		{
			return mRawInputEvent;
		}
#if(_WIN32_WINNT >= _WIN32_WINNT_WIN7)
		TouchEvent& onTouch()
		{
			return mTouchEvent;
		}
#endif
#endif
		PointerDownEvent& onPointerDown()
		{
			return mPointerDownEvent;
		}
		PointerUpEvent& onPointerUp()
		{
			return mPointerUpEvent;
		}

		PointerUpdateEvent& onPointerUpdate()
		{
			return mPointerUpdateEvent;
		}

		PointerWheelEvent& onPointerWheel()
		{
			return mPointerWheelEvent;
		}
		CloseEvent& onClose()
		{
			return mCloseEvent;
		}

	private:
		ActiveEvent mActiveEvent;
		PaintEvent mPaintEvent;
		EnterSizeMoveEvent mEnterSizeMoveEvent;
		ExitSizeMoveEvent mExitSizeMoveEvent;
		SizeEvent mSizeEvent;
		SetCursorEvent mSetCursorEvent;
		CharEvent mCharEvent;
#if defined AIR_PLATFORM_WINDOWS_DESKTOP
		RawInputEvent mRawInputEvent;
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN7)
		TouchEvent mTouchEvent;
#endif
#endif

		PointerDownEvent mPointerDownEvent;
		PointerUpEvent mPointerUpEvent;
		PointerUpdateEvent mPointerUpdateEvent;
		PointerWheelEvent mPointerWheelEvent;
		CloseEvent mCloseEvent;

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
		std::wstring mName;
#if defined AIR_PLATFORM_WINDOWS_DESKTOP
		HWND mWnd;
		WNDPROC mDefaultWndProc;
#endif
#endif
	};
}




#endif