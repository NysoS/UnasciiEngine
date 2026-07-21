#ifndef EXAR_DEF_HPP
#define EXAR_DEF_HPP

#include "Exar/exarpch.h"
#include "Exar/Types.hpp"
#include "Exar/Extent.hpp"

#define DECLARE_EXAR_HANDLE(name) typedef struct name##_ *name;
#define DECLARE_EXAR_HANDLE_INTERFACE(name) typedef struct I##name *name;

namespace Exar {
	DECLARE_EXAR_HANDLE(Memory);
	DECLARE_EXAR_HANDLE(Surface);
	DECLARE_EXAR_HANDLE(Image);
	DECLARE_EXAR_HANDLE(CommandPool);
	DECLARE_EXAR_HANDLE(CommandBuffer);
	DECLARE_EXAR_HANDLE(Cmd);
	DECLARE_EXAR_HANDLE(Device);

#define EXAR_NULL_HANDLE nullptr

	enum class Result
	{
		SUCCESS = 0,
		NULL_POINTER,

		ERROR_INVALID_ARG,

		ERROR_RESOURCES_LOST,
		ERROR_INVALID_FLAG,

		ERROR_INVALID_SIZE,
		ERROR_INVALID_FAMILIES,
		ERROR_INVALID_ALIGN_MEMORY,
		ERROR_MEMORY_MAPING,
		ERROR_MEMORY_MAP_FAILED,
		ERROR_OUT_OF_MEMORY,

		ERROR_INVALID_PAGE_INDEX,
		ERROR_INVALID_PAGE,
		ERROR_ALLOCATOR_NULL_POINTER,
		ERROR_MEMORY_ALLOCATION,
		ERROR_MEMORY_NULL_HANDLE,

		ERROR_MEMORY_CLEANUP
	};

	enum class AreaMemoryType : uint32_t
	{
		NONE = 0,
		SWAPCHAIN,
		COMMAND_POOL,
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

	enum class AllocationMode : uint32_t
	{
		ALLOC_HEAP = 0,
		ALLOC_SCRATCH = 1, // Ring buffer
		ALLOC_DOUBLE_SCRATCH = 2,
		ALLOC_SIMD = 3
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
		COMPUTE = 1,
		TRANSFERT = 2
	};

	enum class CommandPoolFlags : uint32_t 
	{
		MANUAL = 0x0L,
		TRANSIENT_BIT = 0x1L,				// reset command pool
		RESET_COMMAND_BUUFER_BIT = 0x2L,	// reset individual command buffer (by pool)
		RESET_COMMAND_BUFFER_PAGE = 0x4L,
		PROTECTED_BIT = 0x6L				// content protected, DRM type
	};

	enum class CommandType : uint32_t
	{
		TEST = 0,
		NONE = 1,
	};

	enum class CommandBufferState : uint32_t
	{
		INVALID = 0,
		INITIAL = 1,
		RECORDING = 2,
		EXECUTABLE = 3,
		PENDING = 4
	};

	enum CommandBufferUsageFlagBits : uint32_t
	{
		ONE_TIME_SUBMIT_BIT = 0x00000001,
		RENDER_PASS_CONTINUE_BIT = 0x00000002
	};
	typedef uint32_t CommandBufferUsageFlags;

	// no usage here, is all in ram with virtual allocation
	/*enum class CommandResetFlags : uint32_t
	{
		RELEASE_RESOURCES = 0x00000001,
	};*/
}

#endif // EXAR_DEF_HPP
