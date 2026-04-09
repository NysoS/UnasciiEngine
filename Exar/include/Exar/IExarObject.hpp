#ifndef IEXAR_OBJECT_HPP
#define IEXAR_OBJECT_HPP

#include "Exar/Exar.hpp"
#include "Exar/Def.hpp"

namespace Exar {

	class IExarDevice;

	class EXA_API IExarObject
	{
		virtual inline IExarDevice* getDevice() const noexcept = 0;
	};
}

#endif // !IEXAR_OBJECT_HPP
