#ifndef EXAR_AREA_MEMORY_HPP
#define EXAR_AREA_MEMORY_HPP

#include "Exar/MinimalCore.hpp"
#include "Exar/Internal/PageMemory.hpp"
#include "Exar/Descriptor.hpp"

namespace Exar
{
	class AreaMemory
	{
	public:
		explicit AreaMemory();
		~AreaMemory();

		Result createMemory(const AreaMemoryCreateInfo& pAreaCreateInfo, Memory lVram);

		Memory getMemory();
		Memory getMemory() const;
		const PageMemory* getPage(u32 pIndex) const;
		inline const size_t getSize() const noexcept {
			return mSize;
		}

		Result allocate(size_t pSize, u32 pPageIndex = 0);
		Result deallocate(size_t pSize, u32 pPageIndex = 0);

	private:
		Memory mHandle;
		size_t mSize;
		size_t mTotalSize;
		std::vector<PageMemory*> mPages;
	};
}

#endif // !EXAR_AREA_MEMORY_HPP
