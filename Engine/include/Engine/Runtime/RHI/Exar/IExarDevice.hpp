#ifndef IEXAR_DEVICE_HPP //Exarare
#define IEXAR_DEVICE_HPP

namespace UnasciiEngine::RHI::EXAR {
	struct ExarBufferDesc;
	class IExarBuffer;

	class IExarDevice
	{
	public:
		virtual IExarBuffer* createBuffer(const ExarBufferDesc& pDesc) = 0;
		virtual bool GetResourceMemoryRequirements(const IExarBuffer* pBuffer, ExarMemoryRequirement*& pMemRequirement) noexcept = 0;
	};
}

#endif // !EXAR_HPP
