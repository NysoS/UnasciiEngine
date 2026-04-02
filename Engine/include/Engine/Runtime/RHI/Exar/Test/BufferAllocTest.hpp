#ifndef BUFFER_ALLOC_TEST_HPP
#define BUFFER_ALLOC_TEST_HPP

#include "Engine/uaepch.h"
#include "Engine/Engine.hpp"
#include "Engine/Runtime/RHI/Exar/IExarDevice.hpp"

namespace UnasciiEngine::RHI::EXAR {
	class UAE_API BufferAllocTest
	{
	public:
		BufferAllocTest();
		~BufferAllocTest();
	private:
		std::vector<u8> initRandomBufferValue(size_t pSize);

		std::unique_ptr<IExarDevice> mDevice;
	};
}

#endif // !BUFFER_ALLOC_TEST_HPP
