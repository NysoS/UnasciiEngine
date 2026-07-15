#include "Exar/Test/SwapchainPresentTest.hpp"
#include "Exar/ISwapchain.hpp"
#include "Exar/Descriptor.hpp"
#include "Exar/Extent.hpp"
#include "Exar/Device.hpp"
#include "Exar/MinimalCore.hpp"

#ifdef WIN32
#include <Windows.h>
#endif

Exar::SwapchainPresentTest::SwapchainPresentTest()
	: mDevice(std::make_unique<Device_>())
{
	
#ifdef WIN32
	AllocConsole();
#endif

	std::vector<AreaMemoryCreateInfo> lAreasMemeInfo;
	AreaMemoryCreateInfo lAreaMemInfo{};
	lAreaMemInfo.areaType = AreaMemoryType::SWAPCHAIN;
	lAreaMemInfo.mode = MemoryModeFlagBits::READ | MemoryModeFlagBits::WRITE;
	lAreaMemInfo.size = 1024 * 1024 * 1024;
	lAreaMemInfo.pageCount = 2;

	std::vector<PageMemoryCreateInfo> lPagesMemory;
	for (size_t i = 0; i < 2; ++i) {
		PageMemoryCreateInfo lPageMemInfo{};
		lPageMemInfo.size = 512 * 1024 * 1024;
		lPagesMemory.push_back(lPageMemInfo);
	}
	lAreaMemInfo.pageMemory = lPagesMemory.data();
	lAreasMemeInfo.push_back(lAreaMemInfo);
	
	DeviceMemoryCreateInfo lDeviceMemInfo{};
	lDeviceMemInfo.memoryType = MemoryType::AREA;
	lDeviceMemInfo.areaCount = 1;
	lDeviceMemInfo.areaMemory = lAreasMemeInfo.data();

	bool lMemoryCreated = mDevice->createDeviceMemory(lDeviceMemInfo);
	if (!lMemoryCreated)
	{
		std::cerr << "Memory can't created, maybe no space remaining" << std::endl;
		return;
	}

	SwapchainCreateInfo lSwapchainInfo{};
	lSwapchainInfo.extent = Extent2D<u32>{ .w = 600, .h = 600 };
	lSwapchainInfo.minImageCount = 2;
	lSwapchainInfo.imageFormat = PixelFormat::B8G8R8A8_UNORM_SRGB;
	lSwapchainInfo.imageColor = ColorSpace::COLOR_SPACE_SRGB_NONLINEAR;
	lSwapchainInfo.imageUsage = ImageUsageFlags::COLOR_ATTACHMENT;
	lSwapchainInfo.imageSharingMode = SharingMode::EXCLUSIVE;
	lSwapchainInfo.presentMode = PresentationMode::FIFO;

#ifdef WIN32
	HWND hwnd = GetConsoleWindow();
	lSwapchainInfo.surface = reinterpret_cast<Surface>(hwnd);
#endif // WIN32

	Result lRes = mDevice->createSwapchain(lSwapchainInfo, &mSwapchain);
	if (lRes != Result::SUCCESS)
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

	Result lResult = mDevice->destroySwapchain(mSwapchain);
	if (lResult == Result::SUCCESS)
	{
		mSwapchain = nullptr;
	}
}

Exar::SwapchainPresentTest::~SwapchainPresentTest()
{
}
