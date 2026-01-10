#include <iostream>

#include <windows.h>
#include <wchar.h>

#include "Core/Application.hpp"

int main()
{
    //std::cout << "Unascii Engine" << std::endl;

    //const int width = 1200;
    //const int height = 800;

    //const int fontSizeY = 10;

    //AllocConsole();

    //hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
    //if (hstdout == INVALID_HANDLE_VALUE) {
    //    return 1;
    //}

    //CONSOLE_FONT_INFOEX lInfoSize{};
    //lInfoSize.cbSize = sizeof(lInfoSize);
    //lInfoSize.dwFontSize.X = 0; //auto
    //lInfoSize.dwFontSize.Y = fontSizeY;
    //lInfoSize.nFont = 0;
    //wcscpy_s(lInfoSize.FaceName, L"Consolas");
    //SetCurrentConsoleFontEx(hstdout, FALSE, &lInfoSize);

    //CONSOLE_FONT_INFO lSizeInfo;
    //GetCurrentConsoleFont(hstdout, FALSE, &lSizeInfo);
    //std::cout << lSizeInfo.dwFontSize.X << " : " << lSizeInfo.dwFontSize.Y << std::endl;

    //const int bx = width / lSizeInfo.dwFontSize.X;
    //const int by = height / lSizeInfo.dwFontSize.Y;

    //COORD buf{ (SHORT)bx, (SHORT)by };
    //SetConsoleScreenBufferSize(hstdout, buf);

    //SMALL_RECT window{};
    //window.Left = 0;
    //window.Top = 0;
    //window.Right = bx - 1; //120 colonnes
    //window.Bottom = by - 1; //30 lignes
    //SetConsoleWindowInfo(hstdout, TRUE, &window);

    //SetConsoleTextAttribute(hstdout, FOREGROUND_RED);

    Application app;

    return 0;
}