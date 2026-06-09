#include "Exar/Internal/AreaMemory.hpp"

Exar::AreaMemory::AreaMemory(size_t pSize)
	: mHandle(nullptr)
	, mSize(pSize)
{
}

Exar::AreaMemory::~AreaMemory()
{
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
