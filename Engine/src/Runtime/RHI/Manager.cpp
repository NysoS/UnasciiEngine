#include "Engine/Runtime/RHI/Manager.hpp"
#include "Engine/Runtime/RHI/IDeviceFactory.hpp"

std::unique_ptr<UnasciiEngine::RHI::IDeviceFactory> UnasciiEngine::RHI::Manager::getFactory(const std::string& BackendType)
{
	return UnasciiEngine::RHI::Factory::createDeviceFactory();
}
