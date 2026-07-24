#include "Exar/Internal/Allocator.hpp"
#include "Exar/Internal/DeviceMemory.hpp"
#include "Exar/IRessource.hpp"
#include "Exar/Internal/AreaMemory.hpp"
#include "Exar/Internal/PageMemory.hpp"

Exar::Allocator::Allocator()
	: mVram(std::make_unique<DeviceMemory>(0))
	, mOffset(0)
	, mMemorySizeRemaining(0)
{
}

Exar::Allocator::~Allocator()
{
	if (!mVram) return;
	for (const auto& family : mVram->getAreaFamilies()) {
		if (!family) continue;
		VirtualFree(static_cast<void*>(family->getMemory()), 0, MEM_RELEASE);
	}
}

Exar::Result Exar::Allocator::createDeviceMemory(const DeviceMemoryCreateInfo& pDeviceMemoryCreateInfo)
{
	if (pDeviceMemoryCreateInfo.areaCount == 0) return Result::ERROR_INVALID_SIZE;
	if (!pDeviceMemoryCreateInfo.areaMemory) return Result::ERROR_ALLOCATOR_NULL_POINTER;

	EXAR_MEMORY_LOG(stdout, "------- [MEM - Create Device Memory] -------\n");

	for (size_t i = 0; i < pDeviceMemoryCreateInfo.areaCount; ++i) {
		const auto& lArea = pDeviceMemoryCreateInfo.areaMemory[i];

		LOG_ALLOC("Area", (u32)lArea.areaType);
		
		size_t lAreaSize = lArea.size;
		if (lAreaSize == 0) continue;

		void* lMemPtr = VirtualAlloc(NULL, lAreaSize, MEM_RESERVE, PAGE_READWRITE);
		if (!lMemPtr) return Result::ERROR_MEMORY_ALLOCATION; //throw std::exception("Impossible to alloc size on device");

		Result lResult = mVram->createAreaMemory(lArea, (Memory)lMemPtr);
		if (lResult != Result::SUCCESS) return lResult;

		EXAR_MEMORY_LOG(stdout, "------- [MEM - Create Area] -------\n");
		EXAR_MEMORY_LOG(stdout, "------- [Area %u] -------\n", i);
		EXAR_MEMORY_LOG(stdout, "Area size %zu\n", lArea.size);
		EXAR_MEMORY_LOG(stdout, "Area type %u\n", (u32)lArea.areaType);
		EXAR_MEMORY_LOG(stdout, "Area Page Count %u\n", lArea.pageCount);
	}

	return Result::SUCCESS;
}

void* Exar::Allocator::alloc(const AllocatorCreateInfo& pDesc, const MemoryRequirement& pRequirement) noexcept
{
	// Check area type and page index avaiblable
	if (pDesc.areaType == AreaMemoryType::NONE) return nullptr;
	if (pDesc.pageIndex == -1) return nullptr;

	// Get specific area
	auto* lArea = mVram->getAreaMemory(pDesc.areaType);
	if (!lArea) return nullptr;

	const PageMemory* lPage = lArea->getPage(pDesc.pageIndex);
	if (!lPage) return nullptr;

	size_t lMemorySizeRemaining = lPage->size;
	size_t lPageOffset = lPage->offset;

	// calcul offset + padding
	EXAR_MEMORY_LOG(stdout, "------- [Allocation] -------\n");
	size_t lAlignValue = static_cast<size_t>(pRequirement.align);

	EXAR_MEMORY_LOG(stdout, "Area type : %u\n", (u32)pDesc.areaType);
	EXAR_MEMORY_LOG(stdout, "Page index : %u\n", pDesc.pageIndex);
	EXAR_MEMORY_LOG(stdout, "Allocation alignement value : %zu\n", lAlignValue);
	EXAR_MEMORY_LOG(stdout, "Allocation mode : %u\n", (u32)pDesc.mode);
	EXAR_MEMORY_LOG(stdout, "Allocation total size : %u\n", (u32)pDesc.totalSize);
	
	EXAR_MEMORY_LOG(stdout, "Page size remaining : %zu\n", lMemorySizeRemaining);
	EXAR_MEMORY_LOG(stdout, "Page offset : %zu\n", lPageOffset);
	size_t lAlignedOffset = (lPageOffset + (lAlignValue - 1)) & ~(lAlignValue - 1);
	EXAR_MEMORY_LOG(stdout, "Page alignement offset : %zu\n", lAlignedOffset);

	size_t lSizeWithPadding = (lAlignedOffset - lPageOffset) + pRequirement.sizeInBytes;
	EXAR_MEMORY_LOG(stdout, "Data size : %zu\n", pRequirement.sizeInBytes);
	EXAR_MEMORY_LOG(stdout, "Data size with padding : %zu\n", lSizeWithPadding);
	if (lMemorySizeRemaining < lSizeWithPadding) return nullptr;

	// update data page
	Result lAllocPageResult = lArea->allocate(lSizeWithPadding, pDesc.pageIndex);
	if (lAllocPageResult != Result::SUCCESS) return nullptr;

	// alloc data to specific area
	u8* lVRam = reinterpret_cast<u8*>(lArea->getMemory());
	if (!lVRam) return nullptr;
	EXAR_MEMORY_LOG(stdout, "-- [Mem Addresse] --\n");
	EXAR_MEMORY_LOG(stdout, "Area VRam Addresse : %zu\n", (uintptr_t)lVRam);

	u8* lTargetAddr = lVRam + lAlignedOffset;
	EXAR_MEMORY_LOG(stdout, "Adresse target offset : % zu\n", (uintptr_t)lTargetAddr);
	void* lAddMem = VirtualAlloc(lTargetAddr, pRequirement.sizeInBytes, MEM_COMMIT, PAGE_READWRITE);
	if (!lAddMem) return nullptr;

	EXAR_MEMORY_LOG(stdout, "----------------------------\n");

	return lAddMem;
}

bool Exar::Allocator::dealloc(void* pRessource, const AllocatorCreateInfo& pDesc) noexcept
{
	if (pDesc.areaType == AreaMemoryType::NONE || pDesc.pageIndex == -1) return false;

	u8* lRessourceAddr = reinterpret_cast<u8*>(pRessource);
	if (!lRessourceAddr) return false;

	AreaMemory* lArea = mVram->getAreaMemory(pDesc.areaType);
	if (!lArea) return false;

	const PageMemory* lPage = lArea->getPage(pDesc.pageIndex);
	if (!lPage) return false;

	u8* lMemoryAddr = reinterpret_cast<u8*>(lArea->getMemory());
	if (!lMemoryAddr) return false;

	if (!VirtualFree(pRessource, pDesc.totalSize, MEM_DECOMMIT)) return false;

	Result lDeallocPageResult = lArea->deallocate(pDesc.totalSize, pDesc.pageIndex);
	if (lDeallocPageResult != Result::SUCCESS) return false;

	EXAR_MEMORY_LOG(stdout, "------- [Allocation] -------\n");
	EXAR_MEMORY_LOG(stdout, "Ressource Addresse : %zu\n", (uintptr_t)pRessource);
	EXAR_MEMORY_LOG(stdout, "Area type : %u\n", (u32)pDesc.areaType);
	EXAR_MEMORY_LOG(stdout, "Page index : %u\n", pDesc.pageIndex);
	EXAR_MEMORY_LOG(stdout, "Deallocation mode : %u\n", (u32)pDesc.mode);
	EXAR_MEMORY_LOG(stdout, "Deallocation total size : %u\n", (u32)pDesc.totalSize);
	EXAR_MEMORY_LOG(stdout, "----------------------------\n");

	return true;
}

size_t Exar::Allocator::getMemoryAreaSizeRemaining(const AreaMemoryType mAreaType) const noexcept
{
	if (!mVram) return 0;

	const auto* lArea = mVram->getAreaMemory(mAreaType);
	if (!lArea) return 0;

	return lArea->getSize();
}
