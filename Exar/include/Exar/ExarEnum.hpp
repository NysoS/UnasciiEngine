#ifndef EXAR_ENUM_HPP
#define EXAR_ENUM_HPP

#include "Exar/exarpch.h"

namespace Exar {
	enum class ExarUsage : uint32_t
	{
		EXAR_USAGE_DEFAULT = 0,
		EXAR_USAGE_IMMUTABLE,
		EXAR_USAGE_DYNAMIC
	};

	enum class ExarBindFlag : uint32_t
	{
		EXAR_BIND_VERTEX_BUFFER = 0x1L,
		EXAR_BIND_INDEX_BUFFER = 0x2L,
		EXAR_BIND_CONSTANT_BUFFER = 0x4L
	};

	enum class ExarAllocLocation : uint32_t
	{
		EXAR_ALLOC_HEAP = 0,
		EXAR_ALLOC_SCRATCH = 1, // Ring buffer
		EXAR_ALLOC_SIMD = 2
	};

	enum class ExarAlignMemory : size_t
	{
		EXAR_ALIGN_16 = 16,
		EXAR_ALIGN_32 = 32,
		EXAR_ALIGN_64 = 64
	};

	enum class ExarAllocatorType : uint32_t
	{
		EXAR_ALLOCATOR_AREA = 0,
		EXAR_ALLOCATOR_MANUALY
	};

	enum class PixelFormat : uint32_t
	{
		B8G8R8A8_UNORM = 0,		// BGRA
		B8G8R8A8_UNORM_SRGB,	// BGRA sRGB (correction gama only gpu)
		R8G8B8A8_UNORM,			// RGBA
		R8G8B8A8_UNORM_SRGB,	// RGBA sRGB
		R16G16B16A16_FLOAT,		// RGBA 16F
		R32G32B32A32_FLOAT,		// RGBA 32F
		D32_SFLOAT,				// depth 32F
		D24_UNORM_S8_UINT		// depth 24 + stencil 8
	};

	enum class ColorSpace : uint32_t
	{
		COLOR_SPACE_SRGB_NONLINEAR = 0,
		COLOR_SPACE_SRGB_LINEAR
	};

	enum class ImageUsageFlags : uint32_t
	{
		TRANSFER_DST = 0x1L,
		COLOR_ATTACHMENT = 0x2L,
		SAMPLED_BIT = 0x4L
	};

	enum class ImageAspectMask : uint32_t
	{
		COLOR_BIT = 0,
		DEPTH_BIT,
		STENCIL_BIT
	};

	enum class SharingMode : uint32_t
	{
		EXCLUSIVE = 0,
		CONCURRENT
	};

	enum class ImageViewType : uint32_t
	{
		VIEW_1D = 0,
		VIEW_2D,
		VIEW_3D,
		VIEW_CUBE
	};
};

#endif // !EXAR_ENUM_HPP
