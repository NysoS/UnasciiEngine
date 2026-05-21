#include "Exar/Internal/Swapchain.hpp"
#include "Exar/Def.hpp"
#include "Exar/Types.hpp"
#include "Exar/Device.hpp"

#include <Windows.h>

// Buffer de test 16x16 (Damier)
uint32_t testBuffer[600 * 600] = {};
uint32_t testBuffer2[600 * 600] = {};

Exar::Swapchain::Swapchain()
	: mDesc{}
	, mImages(EXAR_NULL_HANDLE)
{
	const uint32_t CELL = 32; // taille d'une case du damier
	for (int y = 0; y < 600; y++) {
		for (int x = 0; x < 600; x++) {
			bool white = ((x / CELL) + (y / CELL)) % 2 == 0;
			testBuffer[y * 600 + x] = white ? 0xFFFFFFFF : 0xFF333333;
		}
	}

	for (int y = 0; y < 600; y++) {
		for (int x = 0; x < 600; x++) {
			bool white = ((x / CELL) + (y / CELL)) % 2 == 0;
			testBuffer2[y * 600 + x] = white ? 0xFFFF0000 : 0xFF0000FF;
		}
	}
}

Exar::Swapchain::~Swapchain()
{
	delete[] mImages;
	mImages = nullptr;
}

Exar::Surface Exar::Swapchain::getSurfaceImageHandle()
{
	return mDesc.surface;
}

void Exar::Swapchain::present()
{
	HWND lHandle = reinterpret_cast<HWND>(mDesc.surface);
	if (!lHandle) return;

	HDC hdc = GetDC(lHandle);

	BITMAPINFO bmi = {};
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = mDesc.extent.w;
	bmi.bmiHeader.biHeight = -static_cast<int32_t>(mDesc.extent.h); // N�gatif pour top-down
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32; // On assume du RGBA 8888
	bmi.bmiHeader.biCompression = BI_RGB;

	void* finalBuffer = nullptr;
	if (index == 0)
	{
		finalBuffer = (void*)testBuffer;
	}
	else
	{
		finalBuffer = (void*)testBuffer2;
	}

	SetDIBitsToDevice(
		hdc,
		0, 0,
		mDesc.extent.w, mDesc.extent.h,
		0,
		0,
		0, 
		mDesc.extent.h,
		finalBuffer,
		&bmi,
		DIB_RGB_COLORS
	);

	ReleaseDC(lHandle, hdc);
}

void Exar::Swapchain::swap() noexcept
{
	index = (index + 1) % 2;
}

Exar::u32 Exar::Swapchain::getImageCount() const noexcept
{
	return mDesc.minImageCount;
}

Exar::Result Exar::Swapchain::getImages(Image* pImages)
{
	for (size_t i = 0; i < mDesc.minImageCount; ++i)
	{
		if (mImages[i] == EXAR_NULL_HANDLE) return Result::NULL_POINTER;

		pImages[i] = mImages[i];
	}

	return Result::SUCCESS;
}

void Exar::Swapchain::initImages()
{
	if (mImages != nullptr) return;

	mImages = new Image[mDesc.minImageCount]();
}
