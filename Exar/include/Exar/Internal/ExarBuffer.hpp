#ifndef EXAR_BUFFER_HPP
#define EXAR_BUFFER_HPP

#include "Exar/IExarBuffer.hpp"
#include "Exar/Def.hpp"

namespace Exar
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
		
		[[nodiscard]] virtual ExarBufferView getView() const override;

		virtual MemoryHandle getCPUAddress() const noexcept override;

		virtual void setCPUAddress(MemoryHandle pData) noexcept override {
			mData = pData;
		}

	private:
		ExarBufferDesc mDesc;
		IExarDevice* mDevice; // raw ptn because not owner memory allocation
		MemoryHandle mData;

		friend class ExarDevice;
	};
}

#endif // !EXAR_BUFFER_HPP
