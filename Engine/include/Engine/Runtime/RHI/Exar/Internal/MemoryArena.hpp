#ifndef MEMORY_ARENA_HPP
#define MEMORY_ARENA_HPP

#include "Engine/Core/Types.hpp"
#include "Engine/Runtime/RHI/RHIDef.hpp"

#include <span>
#include <vector>

namespace UnasciiEngine::RHI::EXAR
{
	struct MemoryRessources
	{
		MemHandle ptr;
		size_t size;
		u32 index;
	};

	class MemoryArena final
	{
	public:
		static MemoryArena& instanceMemory();

	private:
		MemoryArena();
		~MemoryArena();

		MemHandle alloc(std::span<const u8> pData);

		std::vector<MemoryRessources> mRessources;
	};
}

#endif // !MEMORY_ARENA_HPP