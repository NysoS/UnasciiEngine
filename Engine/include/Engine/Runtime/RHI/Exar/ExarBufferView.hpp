#ifndef EXAR_BUFFER_VIEW_HPP
#define EXAR_BUFFER_VIEW_HPP

#include "Engine/uaepch.h"

namespace UnasciiEngine::RHI::EXAR
{
	class ExarBufferView {
		using iterator = const uint8_t*;
		using value_type = uint8_t;
	public:
		ExarBufferView(const uint8_t* pData, size_t pSize)
			: mData(pData)
			, mSize(pSize)
			, mCursor(0)
		{
		}

		inline iterator begin() const noexcept { return mData; }
		inline iterator end() const noexcept { return mData + mSize; }
		inline iterator current() const noexcept { return mData + mCursor; }
		inline bool empty() const noexcept { return mSize == 0; }

		template<typename DataType>
		inline DataType get(size_t pOffset) const noexcept {
			if (pOffset + sizeof(DataType) > mSize) {
				return DataType{};
			}

			return *reinterpret_cast<const DataType*>(mData + pOffset);
		}

	private:
		const uint8_t* mData = nullptr;
		size_t mSize = 0;
		size_t mCursor = 0;
	};
}

#endif // !EXAR_BUFFER_VIEW_HPP