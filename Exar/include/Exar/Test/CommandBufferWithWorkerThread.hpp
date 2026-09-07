#ifndef EXAR_COMMAND_BUFFER_WITH_WORKER_THREAD_HPP
#define EXAR_COMMAND_BUFFER_WITH_WORKER_THREAD_HPP

#include "Exar/MinimalCore.hpp"
#include "Exar/Exar.hpp"
#include "Exar/Descriptor.hpp"

class Device_;

namespace Exar {
	class EXA_API CommandBufferWithWorkerThread
	{
	public:
		CommandBufferWithWorkerThread();
		~CommandBufferWithWorkerThread();
	private:
		void WorkerSystemProcess(std::stop_token pSt, const std::vector<Fence>& pFences);

		std::unique_ptr<Device_> mDevice;

		std::vector<CommandPool> mCommandPools;
		std::vector<AllocatorCreateInfo> mCmdPoolAllocatorInfos;
		std::vector<CommandBuffer> mCommandBuffers;

		std::vector<AllocatorCreateInfo> mFenceAllocatorInfos;
		std::vector<Fence> mFences;
		size_t mCurrentFrame = 0;

		size_t mWorkerFrameProcess = 0;
		std::jthread mWorkerThread;
		};
}

#endif // !EXAR_COMMAND_BUFFER_WITH_WORKER_THREAD_HPP
