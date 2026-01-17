#include "Engine/uaepch.h"
#include "Engine/Runtime/Platform/Windows/WindowWindows.hpp"

#include <windows.h>

namespace UnasciiEngine {
	WindowWindows::WindowWindows(const WindowInfo& pWinInfo)
		: Window()
		, mWinInfo(pWinInfo)
		, mOutHandle(nullptr)
	{
	}

	WindowWindows::~WindowWindows()
	{
	}

	bool WindowWindows::init()
	{
		AllocConsole();

		SetConsoleTitle(mWinInfo.mTitle);

		mOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (mOutHandle == INVALID_HANDLE_VALUE)
		{
			return false;
		}
		
		CONSOLE_FONT_INFOEX lFontInfoEx{};
		lFontInfoEx.cbSize = sizeof(lFontInfoEx);
		lFontInfoEx.dwFontSize.Y = mWinInfo.fontSize;
		lFontInfoEx.nFont = 0;
		wcscpy_s(lFontInfoEx.FaceName, L"Consolas");
		if (!SetCurrentConsoleFontEx(mOutHandle, FALSE, &lFontInfoEx))
		{
			return false;
		}

		CONSOLE_FONT_INFO lSizeInfo;
		if (!GetCurrentConsoleFont(mOutHandle, FALSE, &lSizeInfo))
		{
			return false;
		}

		const int lBufferX = mWinInfo.mWidth / lSizeInfo.dwFontSize.X;
		const int lBufferY = mWinInfo.mHeight / lSizeInfo.dwFontSize.Y;

		COORD lBufferSize{ (SHORT)lBufferX, (SHORT)lBufferY };
		if (!SetConsoleScreenBufferSize(mOutHandle, lBufferSize))
		{
			return false;
		}

		SMALL_RECT lWindow{
			(SHORT)0,
			(SHORT)0,
			(SHORT)(lBufferX - 1),
			(SHORT)(lBufferY - 1)
		};
		if (!SetConsoleWindowInfo(mOutHandle, TRUE, &lWindow))
		{
			return false;
		}

		WORD lForegroundColor = getForegroundColor(mWinInfo.foregroundColor);
		if (!SetConsoleTextAttribute(mOutHandle, lForegroundColor))
		{
			return false;
		}

		return true;
	}

	void WindowWindows::render()
	{
	}

	unsigned short WindowWindows::getForegroundColor(ForegroundColor lColor)
	{
		switch (lColor)
		{
		case ForegroundColor::Blue:
			return FOREGROUND_BLUE;
		case ForegroundColor::Green:
			return FOREGROUND_GREEN;
		case ForegroundColor::Red:
			return FOREGROUND_RED;
		case ForegroundColor::White:
		default:
			return FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
		}

		return 0x0;
	}

	Window* Window::Create(const WindowInfo& pWinInfo)
	{
		return new WindowWindows(pWinInfo);
	}

}