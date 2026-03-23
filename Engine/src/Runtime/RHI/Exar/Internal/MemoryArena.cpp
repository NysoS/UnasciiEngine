#include "Engine/Runtime/RHI/Exar/Internal/MemoryArena.hpp"

#include <memoryapi.h>

UnasciiEngine::RHI::EXAR::MemoryArena& UnasciiEngine::RHI::EXAR::MemoryArena::instanceMemory()
{
	static MemoryArena sMemAren;
	return sMemAren;
}

std::shared_ptr<UnasciiEngine::RHI::EXAR::MemoryArena> UnasciiEngine::RHI::EXAR::MemoryArena::sharedInstanceMemory()
{
	static std::shared_ptr<MemoryArena> sInstance = std::make_shared<MemoryArena>();
	return sInstance;
}

UnasciiEngine::RHI::EXAR::MemoryArena::MemoryArena()
	: mMemory(nullptr)
	, mOffset(0)
	, mMemorySizeReserved(0)
	, mMemorySizeRemaining(0)
{
}

UnasciiEngine::RHI::EXAR::MemoryArena::~MemoryArena()
{
	/*for (auto& lRes : mRessources)
	{
		if (lRes.ptr)
		{
			std::free(lRes.ptr);
			lRes.ptr = nullptr;
		}
	}
	mRessources.clear();*/

	VirtualFree(mMemory, 0, MEM_RELEASE);
}

UnasciiEngine::u8* UnasciiEngine::RHI::EXAR::MemoryArena::alloc(size_t pSize)
{
	if (mMemorySizeRemaining < pSize) return nullptr;

	u8* lMemoryCommit = mMemory + mOffset;

	void* lResult = VirtualAlloc(lMemoryCommit, pSize, MEM_COMMIT, PAGE_READWRITE);

	if (!lResult) return nullptr;

	u8* lRessource = static_cast<u8*>(lResult);

	mOffset += pSize;
	mMemorySizeRemaining -= pSize;

	return lRessource;
}

UnasciiEngine::u8* UnasciiEngine::RHI::EXAR::MemoryArena::allocAligned(size_t pSize, size_t pAlign)
{
	return nullptr;
}

bool UnasciiEngine::RHI::EXAR::MemoryArena::dealloc(size_t pSize, u8*& pRessource)
{
	if (VirtualFree(pRessource, pSize, MEM_DECOMMIT))
	{
		if (pRessource + pSize == mMemory + mOffset)
		{
			mOffset -= pSize;
			mMemorySizeRemaining += pSize;
		}
		pRessource = nullptr;
		return true;
	}

	return false;
}

size_t UnasciiEngine::RHI::EXAR::MemoryArena::getMemorySizeRemaining() const noexcept
{
	return mMemorySizeRemaining;
}

//UnasciiEngine::RHI::MemHandle UnasciiEngine::RHI::EXAR::MemoryArena::alloc(std::span<const u8> pData)
//{
//	size_t lSize = pData.size();
//
//	// alocation with real size of data
//	void* lRawMemory = std::malloc(lSize);
//
//	// check rawptr before copy
//	if (!lRawMemory) return nullptr;
//
//	// copy span data
//	std::memcpy(lRawMemory, pData.data(), lSize);
//
//	// cast ptr on memhandle
//	MemHandle lHandle = static_cast<MemHandle>(lRawMemory);
//
//	MemoryRessources lRessources;
//	lRessources.ptr = lHandle;
//	lRessources.size = lSize;
//	lRessources.index = mRessources.size();
//
//	mRessources.push_back(lRessources);
//
//	return lHandle;
//}
