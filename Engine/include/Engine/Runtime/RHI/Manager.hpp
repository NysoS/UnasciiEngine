#ifndef RHI_MANAGER_HPP
#define RHI_MANAGER_HPP

#include "Engine/uaepch.h"
#include "Engine/Engine.hpp"
#include "Engine/Runtime/RHI/IDeviceFactory.hpp"

namespace UnasciiEngine::RHI
{
	class UAE_API Manager : public IDeviceFactory
	{
		static std::unique_ptr<IDeviceFactory> getFactory(const std::string& BackendType);
	};
}

#endif // !RHI_MANAGER_HPP
