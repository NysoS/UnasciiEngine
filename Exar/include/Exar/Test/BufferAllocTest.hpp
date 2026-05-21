#ifndef BUFFER_ALLOC_TEST_HPP
#define BUFFER_ALLOC_TEST_HPP

#include "Exar/exarpch.h"
#include "Exar/Exar.hpp"
#include "Exar/IDevice.hpp"

namespace Exar {
	class EXA_API BufferAllocTest
	{
	public:
		BufferAllocTest();
		~BufferAllocTest();
	private:
		std::vector<u8> initRandomBufferValue(size_t pSize);

		std::unique_ptr<IDevice> mDevice;
	};
}

#endif // !BUFFER_ALLOC_TEST_HPP
