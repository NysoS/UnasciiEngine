#ifndef IEXAR_DEVICE_HPP //Exarare
#define IEXAR_DEVICE_HPP

#include "Engine/Runtime/RHI/RHIDef.hpp"
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
		virtual void createMemory(size_t pMemorySize) = 0;

		virtual IExarBuffer* createBuffer(const ExarBufferDesc& pDesc) = 0;
		virtual ExarMemoryRequirement getBufferMemoryRequirements(const IExarBuffer* pBuffer) noexcept = 0;
		
		virtual MemHandle allocateResourceMemory(const ExarAllocatorDesc& pDesc, const ExarMemoryRequirement& pRequirement) noexcept = 0;
		
		template <ResourceTypeName ResourceType>
		bool bindResource(ResourceType* pBuffer, MemHandle pResourceMemory)
		{
			if (!pBuffer || !pResourceMemory) return false;

			pBuffer->setData(pResourceMemory);
			return true;
		}
	};
}

#endif // !EXAR_HPP
