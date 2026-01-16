#include "Engine/Core/Application.hpp"
#include "Engine/Runtime/Platform/Window.hpp"

#include <iostream>

namespace UnasciiEngine {
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::createWindow(const WindowInfo& pWinInfo)
	{
		if (!mWindow)
		{
			mWindow = std::unique_ptr<Window>(Window::Create(pWinInfo));
		}
	}

	void Application::run()
	{
		/*previous = getCurrentTime();
		lag = 0.f;*/
		//while (mIsRunning)
		//{
		//	current = getCurrentTime();
		//	elasped = current - previous
		// lag += elasped

		//	mWindow->poolEvent();
		//	while (lag >= MaxFPS)
		//	{
		//		mWindow->update()
		//	}
		//	//render();
		//}

		std::cout << "Running app" << std::endl;
	}

}