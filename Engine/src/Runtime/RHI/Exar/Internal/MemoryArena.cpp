#include "Engine/Runtime/RHI/Exar/Internal/MemoryArena.hpp"

UnasciiEngine::RHI::EXAR::MemoryArena& UnasciiEngine::RHI::EXAR::MemoryArena::instanceMemory()
{
	static MemoryArena sMemAren;
	return sMemAren;
}

UnasciiEngine::RHI::EXAR::MemoryArena::MemoryArena()
	: mRessources({})
{
}

UnasciiEngine::RHI::EXAR::MemoryArena::~MemoryArena()
{
	for (auto& lRes : mRessources)
	{
		if (lRes.ptr)
		{
			std::free(lRes.ptr);
			lRes.ptr = nullptr;
		}
	}
	mRessources.clear();
}

UnasciiEngine::RHI::MemHandle UnasciiEngine::RHI::EXAR::MemoryArena::alloc(std::span<const u8> pData)
{
	size_t lSize = pData.size();

	// alocation with real size of data
	void* lRawMemory = std::malloc(lSize);

	// check rawptr before copy
	if (!lRawMemory) return nullptr;

	// copy span data
	std::memcpy(lRawMemory, pData.data(), lSize);

	// cast ptr on memhandle
	MemHandle lHandle = static_cast<MemHandle>(lRawMemory);

	MemoryRessources lRessources;
	lRessources.ptr = lHandle;
	lRessources.size = lSize;
	lRessources.index = mRessources.size();

	mRessources.push_back(lRessources);

	return lHandle;
}
