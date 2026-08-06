#include "Exar/Internal/AreaMemory.hpp"
#include "Exar/Descriptor.hpp"

#include <iostream>

Exar::AreaMemory::AreaMemory()
	: mHandle(nullptr)
	, mTotalSize(0)
	, mSize(0)
	, mPages()
{
}

Exar::AreaMemory::~AreaMemory()
{
	for (auto& lPage : mPages) {
		if (!lPage) continue;

		delete lPage;
		lPage = nullptr;
	}
}

Exar::Result Exar::AreaMemory::createMemory(const AreaMemoryCreateInfo& pAreaCreateInfo, Memory lVram)
{
	if (pAreaCreateInfo.pageCount == 0 || !pAreaCreateInfo.pageMemory) return Result::ERROR_INVALID_SIZE;

	EXAR_MEMORY_LOG(stdout, "[MEM - Creation Area]\n");

	mPages.resize(pAreaCreateInfo.pageCount);
	PageMemory lLastPageMemory{ 0,0,0,0 };
	for (size_t i = 0; i < pAreaCreateInfo.pageCount; ++i)
	{
		const auto& lInfo = pAreaCreateInfo.pageMemory[i];

		size_t lOffset = lLastPageMemory.size + lLastPageMemory.offset;
		PageMemory* lPage = new PageMemory{ lInfo.size, lInfo.size, lOffset, lOffset };
		mPages[i] = lPage;
		lLastPageMemory = *lPage;

		EXAR_MEMORY_LOG(stdout, "[MEM - Creation Page]\n");
		EXAR_MEMORY_LOG(stdout, "------- [Page] -------\n");
		EXAR_MEMORY_LOG(stdout, "Page totalSize : %zu\n", lInfo.size);
		EXAR_MEMORY_LOG(stdout, "Page size : %zu\n", lInfo.size);
		EXAR_MEMORY_LOG(stdout, "Page startOffset : %zu\n", lOffset);
		EXAR_MEMORY_LOG(stdout, "Page offset : %zu\n", lOffset);
		EXAR_MEMORY_LOG(stdout, "------------------------\n");
	}

	if (!lVram) return Result::ERROR_MEMORY_MAP_FAILED;
	mHandle = lVram;
	mTotalSize = mSize = pAreaCreateInfo.size;

	return Result::SUCCESS;
}

Exar::Memory Exar::AreaMemory::getMemory()
{
	return mHandle;
}

Exar::Memory Exar::AreaMemory::getMemory() const
{
	return mHandle;
}

const Exar::PageMemory* Exar::AreaMemory::getPage(u32 pIndex) const
{
	if (pIndex >= mPages.size()) return	nullptr;

	return mPages.at(pIndex);
}

Exar::Result Exar::AreaMemory::allocate(size_t pSize, u32 pPageIndex)
{
	if (pSize <= 0) return Result::ERROR_INVALID_SIZE;

	if (pPageIndex >= mPages.size()) return Result::ERROR_INVALID_PAGE_INDEX;

	PageMemory* lPage = mPages.at(pPageIndex);
	if (!lPage) return Result::ERROR_INVALID_PAGE;

	if (pSize > mSize || pSize > lPage->size)
		return Result::ERROR_OUT_OF_MEMORY;
	
	mSize -= pSize;
	lPage->size -= pSize;
	lPage->offset += pSize;

	EXAR_MEMORY_LOG(stdout, "------- [Allocation Page Ressource] -------\n");
	EXAR_MEMORY_LOG(stdout, "------- [Page %d] -------\n", pPageIndex);
	EXAR_MEMORY_LOG(stdout, "Page offset : %zu\n", lPage->offset);
	EXAR_MEMORY_LOG(stdout, "Page Size : %zu\n", lPage->size);
	EXAR_MEMORY_LOG(stdout, "----------------------------------------------\n");

	return Result::SUCCESS;
}

Exar::Result Exar::AreaMemory::deallocate(size_t pSize, u32 pPageIndex)
{
	if (pSize <= 0) return Result::ERROR_INVALID_SIZE;

	if (pPageIndex >= mPages.size()) return Result::ERROR_INVALID_PAGE_INDEX;

	PageMemory* lPage = mPages.at(pPageIndex);
	if (!lPage) return Result::ERROR_INVALID_PAGE;

	if ((mSize + pSize) > mTotalSize
		|| (lPage->size + pSize) > lPage->totalSize
		|| (lPage->offset - pSize) < 0)
		return Result::ERROR_OUT_OF_MEMORY;

	mSize += pSize;
	lPage->size += pSize;
	lPage->offset -= pSize;

	EXAR_MEMORY_LOG(stdout, "------- [Deallocate Page Ressource] -------\n");
	EXAR_MEMORY_LOG(stdout, "------- [Page %d] -------\n", pPageIndex);
	EXAR_MEMORY_LOG(stdout, "Page offset : %zu\n", lPage->offset);
	EXAR_MEMORY_LOG(stdout, "Page Size : %zu\n", lPage->size);
	EXAR_MEMORY_LOG(stdout, "----------------------------------------------\n");

	return Result::SUCCESS;
}
