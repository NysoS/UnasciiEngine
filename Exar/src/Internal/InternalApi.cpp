#include "Exar/Internal/InternalApi.hpp"
#include "Exar/"

Exar::Result Exar::Internal::findCommandPool(Device pDevice, const FindRessourceInfo& pFinfo, CommandPool* pCommandPool)
{
	if (!pDevice) return Result::ERROR_MEMORY_NULL_HANDLE;

	return Result::SUCCESS;
}
