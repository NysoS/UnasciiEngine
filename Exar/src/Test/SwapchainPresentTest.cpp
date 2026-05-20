#include "Exar/Test/SwapchainPresentTest.hpp"
#include "Exar/ISwapchain.hpp"
#include "Exar/SwapchainDesc.hpp"
#include "Exar/Extent.hpp"
#include "Exar/ExarEnum.hpp"
#include "Exar/ExarDevice.hpp"

#include <thread>
#include <chrono>

#ifdef WIN32
#include <Windows.h>
#endif

Exar::SwapchainPresentTest::SwapchainPresentTest()
	: mDevice(std::make_unique<ExarDevice>())
{
	
#ifdef WIN32
	AllocConsole();
#endif

	bool lMemoryCreated = mDevice->createMemory(1024 * 1024 * 1024);
	if (!lMemoryCreated)
	{
		std::cerr << "Memory can't created, maybe no space remaining" << std::endl;
		return;
	}

	SwapchainDesc lSwapchainDesc{};
	lSwapchainDesc.extent = Extent2D<u32>{ .w = 600, .h = 600 };
	lSwapchainDesc.minImageCount = 2;
	lSwapchainDesc.imageFormat = PixelFormat::B8G8R8A8_UNORM_SRGB;
	lSwapchainDesc.imageColor = ColorSpace::COLOR_SPACE_SRGB_NONLINEAR;
	lSwapchainDesc.imageUsage = ImageUsageFlags::COLOR_ATTACHMENT;
	lSwapchainDesc.imageSharingMode = SharingMode::EXCLUSIVE;
	lSwapchainDesc.presentMode = PresentationMode::FIFO;

#ifdef WIN32
	HWND hwnd = GetConsoleWindow();
	lSwapchainDesc.surface = reinterpret_cast<Surface>(hwnd);
#endif // WIN32

	ExarResult lRes = mDevice->createSwapchain(lSwapchainDesc, &mSwapchain);
	if (lRes != ExarResult::EXAR_SUCCESS)
	{
		std::cerr << "Error when create swpachain " << (u32)lRes << std::endl;
		return;
	}

	u32 lMaxTimingLoop = 120;
	u32 lTime = 0;
	while (lTime < lMaxTimingLoop)
	{
		std::this_thread::sleep_for(std::chrono::seconds(2));

		mSwapchain->present();
		mSwapchain->swap();

		lTime++;
	}

	ExarResult lResult = mDevice->destroySwapchain(mSwapchain);
	if (lResult == ExarResult::EXAR_SUCCESS)
	{
		mSwapchain = nullptr;
	}
}

Exar::SwapchainPresentTest::~SwapchainPresentTest()
{
}
