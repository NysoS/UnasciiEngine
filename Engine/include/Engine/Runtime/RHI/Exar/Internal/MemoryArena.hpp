//#ifndef MEMORY_ARENA_HPP
//#define MEMORY_ARENA_HPP
//
//#include "Engine/Core/Types.hpp"
//
//#include <span>
//#include <vector>
//
//namespace UnasciiEngine::RHI::EXAR
//{
//	class MemoryArena final
//	{
//	public:
//		static MemoryArena& instanceMemory();
//		static std::shared_ptr<MemoryArena> sharedInstanceMemory();
//
//	private:
//		MemoryArena();
//		~MemoryArena();
//
//		// pRessource is reference
//		u8* alloc(size_t pSize);
//		u8* allocAligned(size_t pSize, size_t pAlign);
//		bool dealloc(size_t pSize, u8*& pRessource);
//		
//		size_t getMemorySizeRemaining() const noexcept;
//
//		u8* mMemory; // Fake VRam reserved
//		size_t mOffset;
//		size_t mMemorySizeReserved;
//		size_t mMemorySizeRemaining;
//
//		friend class ExarAllocator;
//		friend class ExarDevice;
//	};
//}
//
//#endif // !MEMORY_ARENA_HPP