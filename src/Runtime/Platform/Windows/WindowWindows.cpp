#include "Runtime/Platform/Windows/WindowWindows.hpp"

#include <windows.h>
#include <wchar.h>

WindowWindows::WindowWindows(const WindowInfo& lWinInfo)
	: Window()
{
	if (!init(lWinInfo))
	{
		throw;
	}
}

WindowWindows::~WindowWindows()
{
}

bool WindowWindows::init(const WindowInfo& lWinInfo)
{
	AllocConsole();

	mOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (mOutHandle == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	CONSOLE_FONT_INFOEX lFontInfoEx{};
	lFontInfoEx.cbSize = sizeof(lFontInfoEx);
	lFontInfoEx.dwFontSize.Y = lWinInfo.fontSize;
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

	const int lBufferX = lWinInfo.mWidth / lSizeInfo.dwFontSize.X;
	const int lBufferY = lWinInfo.mHeight / lSizeInfo.dwFontSize.Y;

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

	WORD lForegroundColor = getForegroundColor(lWinInfo.foregroundColor);
	if (!SetConsoleTextAttribute(mOutHandle, lForegroundColor))
	{
		return false;
	}

	return true;
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

Window* Window::Create(const WindowInfo& lWinInfo)
{
	return new WindowWindows(lWinInfo);
}
