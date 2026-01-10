#include "Core/Application.hpp"

#include "Runtime/Platform/Window.hpp"

Application::Application()
{
	WindowInfo lWindInfo;
	lWindInfo.foregroundColor = ForegroundColor::Red;
	
	mWindow = std::unique_ptr<Window>(Window::Create(lWindInfo));
}

Application::~Application()
{
}

void Application::run()
{
}
