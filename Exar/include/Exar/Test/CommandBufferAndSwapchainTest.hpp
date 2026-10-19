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

		void workerThreadProcess(std::stop_token pSt, const std::vector<Fence>& pFences);
		void displayThreadProcess(std::stop_token pSt, const std::vector<Semaphore>& pImageAvailableSemaphores, const std::vector<Semaphore>& pRenderFinishedSemaphores);

		std::unique_ptr<Device_> mDevice;
		std::vector<CommandPool> mCommandPools;
		std::vector<AllocatorCreateInfo> mCmdPoolAllocatorInfos;
		std::vector<CommandBuffer> mCommandBuffers;
		
		Swapchain mSwapchain;
		std::vector<Image> mImages;
		AllocatorCreateInfo mImageAllocatorInfos;
		std::vector<ImageView> mImagesViews;
		std::vector<AllocatorCreateInfo> mImageViewAllocatorInfos;

		std::vector<Fence> mInFlightFences;
		std::vector<Fence> mImagesInFlight;
		std::vector<AllocatorCreateInfo> mInFlightFencesAllocatorInfo;

		std::vector<Semaphore> mImageAvailableSemaphores;
		std::vector<AllocatorCreateInfo> mImageAvailableSemaphoreAllocatorInfo;
		std::vector<Semaphore> mRenderFinishedSemaphores;
		std::vector<AllocatorCreateInfo> mRenderFinishedSemaphoreAllocatorInfo;

		size_t mMainFrameIndex = 0;
		size_t mWorkerFrameIndex = 0;
		size_t mDisplayThreadIndex = 0;

		std::jthread mWorkerThread;
		std::jthread mDisplayThread;
	};
}
#endif // !EXAR_COMMAND_BUFFER_AND_SWAPCHAIN_TEST_HPP
