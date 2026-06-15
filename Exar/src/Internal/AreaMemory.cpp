#include "Exar/Internal/AreaMemory.hpp"
#include "Exar/Descriptor.hpp"

#include <iostream>

Exar::AreaMemory::AreaMemory()
	: mHandle(nullptr)
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

	std::cout << "create Area" << std::endl;

	mPages.resize(pAreaCreateInfo.pageCount);
	PageMemory lLastPageMemory{ 0,0 };
	for (size_t i = 0; i < pAreaCreateInfo.pageCount; ++i)
	{
		std::cout << "create Page : " << i << std::endl;
		const auto& lInfo = pAreaCreateInfo.pageMemory[i];

		size_t lOffset = lLastPageMemory.size + lLastPageMemory.offset;
		PageMemory* lPage = new PageMemory{ lInfo.size, lOffset };
		mPages[i] = lPage;
		lLastPageMemory = *lPage;
	}

	if (!lVram) return Result::ERROR_MEMORY_MAP_FAILED;
	mHandle = lVram;
	mSize = pAreaCreateInfo.size;

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

bool Exar::AreaMemory::updateMemory(size_t pSize, u32 pPageIndex)
{
	if (mSize - pSize < 0) return false;

	if (pPageIndex > mPages.size()) return false;

	PageMemory* lPage = mPages.at(pPageIndex);
	if (lPage->size - pSize < 0) return false;

	mSize -= pSize;
	lPage->size -= pSize;
	lPage->offset += pSize;

	std::cout << "Page : [ " << pPageIndex << " ] " << std::endl;
	std::cout << "New offset" << " [ " << lPage->offset << " ] " << std::endl;
	std::cout << "Memory Size Remaining after allocation" << " [ " << lPage->size << " ]" << std::endl;

	return true;
}
