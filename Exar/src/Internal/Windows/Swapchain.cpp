#include "Exar/Internal/Swapchain.hpp"
#include "Exar/MinimalCore.hpp"
#include "Exar/Device.hpp"
#include "Exar/Semaphore.hpp"
#include "Exar/Internal/ResourceTypes.h"

#include <Windows.h>

// Buffer de test 16x16 (Damier)
//uint32_t testBuffer[600 * 600] = {};
//uint32_t testBuffer2[600 * 600] = {};

Exar::Swapchain_::Swapchain_()
	: mInfo{}
	, mImages(EXAR_NULL_HANDLE)
{
	//const uint32_t CELL = 32; // taille d'une case du damier
	//for (int y = 0; y < 600; y++) {
	//	for (int x = 0; x < 600; x++) {
	//		bool white = ((x / CELL) + (y / CELL)) % 2 == 0;
	//		testBuffer[y * 600 + x] = white ? 0xFFFFFFFF : 0xFF333333;
	//	}
	//}

	//for (int y = 0; y < 600; y++) {
	//	for (int x = 0; x < 600; x++) {
	//		bool white = ((x / CELL) + (y / CELL)) % 2 == 0;
	//		testBuffer2[y * 600 + x] = white ? 0xFFFF0000 : 0xFF0000FF;
	//	}
	//}
}

Exar::Swapchain_::~Swapchain_()
{
}

Exar::Surface Exar::Swapchain_::getSurfaceImageHandle()
{
	return mInfo.surface;
}

void Exar::Swapchain_::present(u32 pIndex)
{
	HWND lHandle = reinterpret_cast<HWND>(mInfo.surface);
	if (!lHandle) return;

	HDC hdc = GetDC(lHandle);

	BITMAPINFO bmi = {};
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = mInfo.extent.w;
	bmi.bmiHeader.biHeight = -static_cast<int32_t>(mInfo.extent.h); // N�gatif pour top-down
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32; // On assume du RGBA 8888
	bmi.bmiHeader.biCompression = BI_RGB;

	void* finalBuffer = nullptr;
	/*if (mIndex == 0)
	{
		finalBuffer = (void*)testBuffer;
	}
	else
	{
		finalBuffer = (void*)testBuffer2;
	}*/

	finalBuffer = mImages[pIndex]->pixels;

	SetDIBitsToDevice(
		hdc,
		0, 0,
		mInfo.extent.w, mInfo.extent.h,
		0,
		0,
		0, 
		mInfo.extent.h,
		finalBuffer,
		&bmi,
		DIB_RGB_COLORS
	);

	ReleaseDC(lHandle, hdc);
}

void Exar::Swapchain_::swap() noexcept
{
	mIndex = (mIndex + 1) % 2;
}

Exar::u32 Exar::Swapchain_::getImageCount() const noexcept
{
	return mInfo.minImageCount;
}

Exar::Result Exar::Swapchain_::getImages(Image* pImages)
{
	for (size_t i = 0; i < mInfo.minImageCount; ++i)
	{
		if (mImages[i] == EXAR_NULL_HANDLE) return Result::NULL_POINTER;

		pImages[i] = mImages[i];
	}

	return Result::SUCCESS;
}

void Exar::Swapchain_::initImages()
{
	/*if (mImages != nullptr) return;

	mImages = new Image[mInfo.minImageCount]();*/
}

Exar::Result Exar::Swapchain_::acquireIndex(Semaphore pSemaphore, u32* pIndex) noexcept
{
	std::lock_guard<std::mutex> lLoc(mAcquireMutex);
	u32 lIndex = mIndex;
	
	assert((lIndex >= mImageUsed.size() && lIndex >= mWaitingImagesSemaphores.size()) && "index is out of range");
	if (lIndex >= mImageUsed.size() || lIndex >= mWaitingImagesSemaphores.size()) return Result::ERROR_INVALID_SIZE;

	swap();

	if (!mImageUsed[lIndex])
	{
		mWaitingImagesSemaphores[lIndex]->release();
	}
	else {
		mWaitingImagesSemaphores[lIndex] = pSemaphore;
	}
	mImageUsed[lIndex] = true;

	*pIndex = lIndex;

	return Result::SUCCESS;
}

Exar::Result Exar::Swapchain_::releaseIndex(u32 pIndex) noexcept
{
	std::lock_guard<std::mutex> lLoc(mAcquireMutex);
	assert((pIndex >= mImageUsed.size() && pIndex >= mWaitingImagesSemaphores.size()) && "index is out of range");
	if (pIndex >= mImageUsed.size() || pIndex >= mWaitingImagesSemaphores.size()) return Result::ERROR_INVALID_SIZE;

	if (mWaitingImagesSemaphores[pIndex])
	{
		mWaitingImagesSemaphores[pIndex]->release();
		mWaitingImagesSemaphores[pIndex] = nullptr;
	}
	else 
	{
		mImageUsed.at(pIndex) = false;
	}
}