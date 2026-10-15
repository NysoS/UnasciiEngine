#ifndef EXAR_COMMAND_BUFFER_AND_SWAPCHAIN_TEST_HPP
#define EXAR_COMMAND_BUFFER_AND_SWAPCHAIN_TEST_HPP

#include "Exar/MinimalCore.hpp"
#include "Exar/Exar.hpp"
#include "Exar/Descriptor.hpp"

class Device_;

namespace Exar {
	class EXA_API CommandBufferAndSwapchainTest
	{
	public:
		CommandBufferAndSwapchainTest();
		~CommandBufferAndSwapchainTest();
	private:
		bool initMemory();
		bool init();

		std::unique_ptr<Device_> mDevice;
		std::vector<CommandPool> mCommandPools;
		std::vector<AllocatorCreateInfo> mCmdPoolAllocatorInfos;
		std::vector<CommandBuffer> mCommandBuffers;
		
		Swapchain mSwapchain;
		std::vector<Image> mImages;
		AllocatorCreateInfo mImageAllocatorInfos;
		std::vector<ImageView> mImagesViews;
		std::vector<AllocatorCreateInfo> mImageViewAllocatorInfos;
	};
}
#endif // !EXAR_COMMAND_BUFFER_AND_SWAPCHAIN_TEST_HPP
