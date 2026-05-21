#ifndef IEXAR_RESSOURCE_HPP
#define IEXAR_RESSOURCE_HPP

#include "Exar/IObject.hpp"

namespace Exar {
	class EXA_API IRessource : public IObject
	{
	public:
		virtual MemoryHandle getCPUAddress() const noexcept = 0;
		virtual void setCPUAddress(MemoryHandle pData) noexcept = 0;
	};
}

#endif // !IEXAR_RESSOURCE_HPP
