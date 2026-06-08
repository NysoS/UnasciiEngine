#ifndef EXAR_DEF_HPP
#define EXAR_DEF_HPP

#include "Exar/exarpch.h"
#include "Exar/Types.hpp"
#include "Exar/Extent.hpp"

#define DECLARE_EXAR_HANDLE(name) typedef struct name##_ *name;

namespace Exar {
	DECLARE_EXAR_HANDLE(Memory);
	DECLARE_EXAR_HANDLE(Surface);
	DECLARE_EXAR_HANDLE(Image);

#define EXAR_NULL_HANDLE nullptr

	enum class Result
	{
		SUCCESS = 0,
		NULL_POINTER,

		ERROR_INVALID_ARG,

		ERROR_RESOURCES_LOST,

		ERROR_INVALID_SIZE,
		ERROR_INVALID_ALIGN_MEMORY,
		ERROR_MEMORY_MAP_FAILED,
		ERROR_OUT_OF_MEMORY,

		ERROR_INVALID_PAGE_INDEX,
		ERROR_INVALID_PAGE,
		ERROR_ALLOCATOR_NULL_POINTER,
		ERROR_MEMORY_ALLOCATION
	};

	enum class AreaMemoryType : uint32_t
	{
		NONE = 0,
		SWAPCHAIN,
		COMMAND_BUFFER,
		TEXTURE
	};

	enum class MemoryType : uint32_t
	{
		AREA = 0,
		BLOC
	};

	enum MemoryModeFlagBits : uint32_t
	{
		READ = 0x1,
		WRITE = 0x2,
		RESERVED = 0x4 // Create reserved bloc
	};
	typedef uint32_t MemoryModeFlags;

	enum class Usage : uint32_t
	{
		DEFAULT = 0,
		IMMUTABLE,
		DYNAMIC
	};

	enum class BindFlag : uint32_t
	{
		BIND_VERTEX_BUFFER = 0x1L,
		BIND_INDEX_BUFFER = 0x2L,
		BIND_CONSTANT_BUFFER = 0x4L
	};

	enum class AllocLocation : uint32_t
	{
		ALLOC_HEAP = 0,
		ALLOC_SCRATCH = 1, // Ring buffer
		ALLOC_SIMD = 2
	};

	enum class AlignMemory : size_t
	{
		ALIGN_16 = 16,
		ALIGN_32 = 32,
		ALIGN_64 = 64
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

	enum class PresentationMode : uint32_t
	{
		IMMEDIATE = 0,
		FIFO,
		FIFO_RELAXED,
		MAILBOX
	};

	enum class ImageUsageFlags : uint32_t
	{
		TRANSFER_DST = 0x1,
		COLOR_ATTACHMENT = 0x2,
		SAMPLED_BIT = 0x4
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

	enum class ImageAspectMask : uint32_t
	{
		COLOR_BIT = 0,
		DEPTH_BIT,
		STENCIL_BIT
	};

	enum class AttachmentLoadOp : uint32_t
	{
		LOAD_OP_LOAD = 0,
		LOAD_OP_CLEAR = 1,
		LOAD_OP_DONT_CARE = 2,
		LOAD_OP_NONE = 1000400000
	};

	enum class AttachmentStoreOp : uint32_t
	{
		STORE_OP_STORE = 0,
		STORE_OP_DONT_CARE = 1,
		STORE_OP_NONE = 1000301000
	};

	enum class ImageLayout : uint32_t
	{
		UNDEFINED = 0,
		PRESENT_SRC = 1,
		COLOR_ATTACHMENT_OPTIMAL = 2
	};

	enum class PipelineBindPoint : uint32_t
	{
		GRAPHICS = 0,
		COMPUTE
	};

	enum class QueueFamily : uint32_t 
	{
		GRAPHICS = 0,
		COMPURE = 1,
		TRANSFERT = 2
	};

	enum class CommandPoolFlags :uint32_t 
	{
		TRANSIENT_BIT = 0x1L,				// reset command pool
		RESET_COMMAND_BUUFER_BIT = 0x2L,	// reset individual command buffer
		PROTECTED_BIT = 0x4L				// content protected, DRM type
	};
}

#endif // EXAR_DEF_HPP
