#ifndef EXAR_SWAPCHAIN_PRESENT_TEST_HPP
#define EXAR_SWAPCHAIN_PRESENT_TEST_HPP

#include "Exar/MinimalCore.hpp"
#include "Exar/Exar.hpp"

namespace Exar {
	class EXA_API SwapchainPresentTest
	{
	public:
		SwapchainPresentTest();
		~SwapchainPresentTest();
	private:
		Device mDevice;
		class ISwapchain* mSwapchain;
	};
}

#endif // !EXAR_SWAPCHAIN_PRESENT_TEST_HPP
