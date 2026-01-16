#include <iostream>

#include <windows.h>
#include <wchar.h>

#include <Engine/Core/Application.hpp>

int main()
{
    UnasciiEngine::WindowInfo lInfo;
    lInfo.foregroundColor = UnasciiEngine::ForegroundColor::Red;

    UnasciiEngine::Application app(lInfo);
    app.init();
    app.run();

    return 0;
}