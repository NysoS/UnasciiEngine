#ifndef IEXAR_OBJECT_HPP
#define IEXAR_OBJECT_HPP

#include "Exar/Exar.hpp"
#include "Exar/Def.hpp"

namespace Exar {

	class IDevice;

	class EXA_API IObject
	{
		virtual inline IDevice* getDevice() const noexcept = 0;
	};
}

#endif // !IEXAR_OBJECT_HPP
