#ifndef EXAR_SWAPCHAIN_PRESENT_TEST_HPP
#define EXAR_SWAPCHAIN_PRESENT_TEST_HPP

#include "Exar/exarpch.h"
#include "Exar/Exar.hpp"
#include "Exar/IDevice.hpp"

namespace Exar {
	class EXA_API SwapchainPresentTest
	{
	public:
		SwapchainPresentTest();
		~SwapchainPresentTest();
	private:
		std::unique_ptr<IDevice> mDevice;
		class ISwapchain* mSwapchain;
	};
}

#endif // !EXAR_SWAPCHAIN_PRESENT_TEST_HPP
