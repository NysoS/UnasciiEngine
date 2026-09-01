#ifndef EXAR_INTERNAL_API_HPP
#define EXAR_INTERNAL_API_HPP

#include "Exar/Core.hpp"

namespace Exar::Internal
{
	Result findCommandPool(Device pDevice, const FindRessourceInfo& pFinfo, CommandPool* pCommandPool);
}

#endif // !EXAR_INTERNAL_API_HPP
