#ifndef EXAR_AREA_MEMORY_HPP
#define EXAR_AREA_MEMORY_HPP

#include "Exar/MinimalCore.hpp"
#include "Exar/Internal/PageMemory.hpp"

namespace Exar
{
	class AreaMemory
	{
	public:
		explicit AreaMemory(size_t pSize);
		~AreaMemory();

		Memory getMemory();
		const PageMemory getPage(u32 pIndex) const;

		bool updateMemory(size_t pSize, u32 pPageIndex = 0);

	private:
		Memory mHandle;
		size_t mSize;
		std::vector<PageMemory> mPages;
	};
}

#endif // !EXAR_AREA_MEMORY_HPP
