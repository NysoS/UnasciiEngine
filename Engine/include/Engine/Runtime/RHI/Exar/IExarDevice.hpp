#ifndef IEXAR_DEVICE_HPP //Exarare
#define IEXAR_DEVICE_HPP

namespace UnasciiEngine::RHI::EXAR {
	struct ExarBufferDesc;
	class IExarBuffer;

	class IExarDevice
	{
		virtual IExarBuffer* createBuffer(const ExarBufferDesc& pDesc) = 0;
	};
}

#endif // !EXAR_HPP
