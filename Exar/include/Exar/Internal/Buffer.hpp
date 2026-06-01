#ifndef EXAR_BUFFER_HPP
#define EXAR_BUFFER_HPP

#include "Exar/IBuffer.hpp"
#include "Exar/MinimalCore.hpp"

namespace Exar
{
	class BufferView;
	
	class Buffer : public IBuffer 
	{
	public:
		Buffer();
		virtual ~Buffer();

		Buffer(Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;

		virtual BufferDesc getDesc() const noexcept;
		virtual inline IDevice* getDevice() const noexcept override {
			return mDevice;
		}
		
		[[nodiscard]] virtual BufferView getView() const override;

		virtual MemoryHandle getCPUAddress() const noexcept override;

		virtual void setCPUAddress(MemoryHandle pData) noexcept override {
			mData = pData;
		}

	private:
		BufferDesc mDesc;
		IDevice* mDevice; // raw ptn because not owner memory allocation
		MemoryHandle mData;

		friend class Device;
	};
}

#endif // !EXAR_BUFFER_HPP
