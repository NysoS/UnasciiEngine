#include "Engine/uaepch.h"
#include "Engine/Core/Application.hpp"
#include "Engine/Runtime/Platform/Window.hpp"

#include "Engine/Runtime/RHI/Manager.hpp"
#include "Engine/Runtime/RHI/Test/IRHITest.hpp"

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
		mWindow->init();
		// //std::unique_ptr<RHI::Test::IRHIBufferAllocationTest> lBufferAllocTest = RHI::Factory::createBufferAllocationTest();
		 //std::unique_ptr<RHI::Test::IRHISwapchainTest> lSwapchainPresentTest = RHI::Factory::createSwapchainPresentTest();
		 //lSwapchainPresentTest->execute();

		//std::unique_ptr<RHI::Test::IRHICommandPoolTest> lCommandPoolTest = RHI::Factory::createCommandPoolAllocationTest();
		std::unique_ptr<RHI::Test::IRHICommandBufferWithWorkerThread> lCommandBufferTest = RHI::Factory::createCommandBufferWithWorkerThread();
		lCommandBufferTest->execute();
		return true;
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