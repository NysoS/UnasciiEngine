#include "Exar/Internal/AreaMemory.hpp"
#include "Exar/Descriptor.hpp"

#include <iostream>

Exar::AreaMemory::AreaMemory()
	: mHandle(nullptr)
	, mSize(0)
	, mPages({})
{
}

Exar::AreaMemory::~AreaMemory()
{
}

Exar::Result Exar::AreaMemory::createMemory(const AreaMemoryCreateInfo& pAreaCreateInfo, Memory lVram)
{
	if (pAreaCreateInfo.pageCount == 0 || !pAreaCreateInfo.pageMemory) return Result::ERROR_INVALID_SIZE;

	std::cout << "create Area" << std::endl;

	mPages.resize(pAreaCreateInfo.pageCount);
	for (size_t i = 0; i < pAreaCreateInfo.pageCount; ++i)
	{
		std::cout << "create Page : " << i << std::endl;

		const auto& lInfo = pAreaCreateInfo.pageMemory[i];

		mPages.emplace_back(lInfo.size, 0);
	}

	return Result::SUCCESS;
}

Exar::Memory Exar::AreaMemory::getMemory()
{
	return mHandle;
}

const Exar::PageMemory Exar::AreaMemory::getPage(u32 pIndex) const
{
	return mPages.at(pIndex);
}

bool Exar::AreaMemory::updateMemory(size_t pSize, u32 pPageIndex)
{
	if (mSize - pSize < 0) return false;

	if (pPageIndex > mPages.size()) return false;

	PageMemory lPage = mPages.at(pPageIndex);
	if (lPage.size - pSize < 0) return false;

	mSize -= pSize;
	lPage.size -= pSize;
	lPage.offset += pSize;

	return true;
}
