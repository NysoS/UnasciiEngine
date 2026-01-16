#include "Engine/Core/Application.hpp"
#include "Engine/Runtime/Platform/Window.hpp"

#include <iostream>

namespace UnasciiEngine {
	Application::Application(const WindowInfo& pWinInfo)
		: mWindow(nullptr)
		, mIsRunning(false)
	{
		if (!mWindow)
		{
			mWindow = std::unique_ptr<Window>(Window::Create(pWinInfo));
		}
	}

	Application::~Application()
	{
	}

	bool Application::init()
	{
		return mWindow->init();
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