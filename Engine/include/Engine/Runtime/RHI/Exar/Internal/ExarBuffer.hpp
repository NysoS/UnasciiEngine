#ifndef EXAR_BUFFER_HPP
#define EXAR_BUFFER_HPP

#include "Engine/Runtime/RHI/Exar/IExarBuffer.hpp"
#include "Engine/Runtime/RHI/RHIDef.hpp"

namespace UnasciiEngine::RHI::EXAR
{
	class ExarBuffer : IExarBuffer 
	{
	public:
		ExarBuffer();
		virtual ~ExarBuffer();

		ExarBuffer(ExarBuffer&) = delete;
		ExarBuffer& operator=(const ExarBuffer&) = delete;

		virtual ExarBufferDesc getDesc() const noexcept;
		virtual RHIHandle getDevice() const noexcept;
		MemHandle getData() const noexcept;

	private:
		ExarBufferDesc mDesc;
		RHIHandle mDevice; // raw ptn because now owner memory allocation
		MemHandle mData;
	};
}

#endif // !EXAR_BUFFER_HPP
