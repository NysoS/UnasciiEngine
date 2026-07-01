#ifndef IRHI_TEST_HPP
#define IRHI_TEST_HPP

#include "Engine/Engine.hpp"

namespace UnasciiEngine::RHI::Test
{
	class UAE_API IRHIBufferAllocationTest
	{
	public:
		virtual ~IRHIBufferAllocationTest() = default;
		virtual void execute() = 0;
	};

	class UAE_API IRHISwapchainTest
	{
	public:
		virtual ~IRHISwapchainTest() = default;
		virtual void execute() = 0;
	};

	class UAE_API IRHICommandPoolTest
	{
	public:
		virtual ~IRHICommandPoolTest() = default;
		virtual void execute() = 0;
	};
}

#endif // !IRHI_TEST_HPP
