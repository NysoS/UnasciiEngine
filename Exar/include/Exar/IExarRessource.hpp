#ifndef IEXAR_RESSOURCE_HPP
#define IEXAR_RESSOURCE_HPP

#include "Exar/IExarObject.hpp"

namespace Exar {
	class EXA_API IExarRessource : public IExarObject
	{
	public:
		virtual MemoryHandle getCPUAddress() const noexcept = 0;
		virtual void setCPUAddress(MemoryHandle pData) noexcept = 0;
	};
}

#endif // !IEXAR_RESSOURCE_HPP
