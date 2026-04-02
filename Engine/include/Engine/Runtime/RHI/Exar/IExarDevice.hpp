#ifndef IEXAR_DEVICE_HPP //Exarare
#define IEXAR_DEVICE_HPP

#include "Engine/uaepch.h"
#include "Engine/Runtime/RHI/RHIDef.hpp"
#include "Engine/Core/Types.hpp"
#include "Engine/Runtime/RHI/Exar/ExarMemoryRequirements.hpp"

namespace UnasciiEngine::RHI::EXAR {
	struct ExarBufferDesc;
	class IExarBuffer;
	class IExarRessource;
	struct ExarAllocatorDesc;

	template <typename Type>
	concept ResourceTypeName = std::derived_from<Type, IExarRessource>;

	class IExarDevice
	{
	public:
		virtual ~IExarDevice() {};
		virtual bool createMemory(size_t pMemorySize) = 0;

		virtual IExarBuffer* createBuffer(const ExarBufferDesc& pDesc) = 0;

		virtual ExarMemoryRequirement getBufferMemoryRequirements(const IExarBuffer* pBuffer) noexcept = 0;
		
		virtual MemHandle allocateResourceMemory(const ExarAllocatorDesc& pDesc, const ExarMemoryRequirement& pRequirement) noexcept = 0;
		
		virtual bool updateResourceData(MemHandle& pMemHandle, const ExarMemoryRequirement& pRequirement, std::span<const u8> pData) noexcept = 0;

		template <ResourceTypeName ResourceType>
		bool bindResource(ResourceType* pBuffer, MemHandle pResourceMemory)
		{
			if (!pBuffer || !pResourceMemory) return false;

			pBuffer->setCPUAddress(pResourceMemory);
			return true;
		}
	};
}

#endif // !EXAR_HPP
