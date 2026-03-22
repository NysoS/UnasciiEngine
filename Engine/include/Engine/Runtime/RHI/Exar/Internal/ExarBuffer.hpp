#ifndef EXAR_BUFFER_HPP
#define EXAR_BUFFER_HPP

#include "Engine/Runtime/RHI/Exar/IExarBuffer.hpp"
#include "Engine/Runtime/RHI/RHIDef.hpp"

namespace UnasciiEngine::RHI::EXAR
{
	class ExarBufferView;
	
	class ExarBuffer : public IExarBuffer 
	{
	public:
		ExarBuffer();
		virtual ~ExarBuffer();

		ExarBuffer(ExarBuffer&) = delete;
		ExarBuffer& operator=(const ExarBuffer&) = delete;

		virtual ExarBufferDesc getDesc() const noexcept;
		virtual inline IExarDevice* getDevice() const noexcept override {
			return mDevice;
		}
		MemHandle getData() const noexcept;
		[[nodiscard]] ExarBufferView getView() const;

	private:
		ExarBufferDesc mDesc;
		IExarDevice* mDevice; // raw ptn because not owner memory allocation
		MemHandle mData;

		friend class ExarDevice;
	};
}

#endif // !EXAR_BUFFER_HPP
