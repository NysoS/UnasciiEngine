#ifndef IEXAR_RESSOURCE_HPP
#define IEXAR_RESSOURCE_HPP

#include "Exar.hpp"
#include "IExarObject.hpp"

namespace UnasciiEngine::RHI::EXAR {
	class IExarRessource : public IExarObject
	{
	public:
		virtual MemHandle getCPUAddress() const noexcept = 0;
		virtual void setData(MemHandle pData) noexcept = 0;
	};
}

#endif // !IEXAR_RESSOURCE_HPP
