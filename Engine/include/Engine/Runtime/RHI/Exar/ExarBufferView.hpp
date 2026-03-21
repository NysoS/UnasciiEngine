#ifndef EXAR_BUFFER_VIEW_HPP
#define EXAR_BUFFER_VIEW_HPP

#include "Engine/uaepch.h"
#include "Engine/Core/Types.hpp"

#include <span>

namespace UnasciiEngine::RHI::EXAR
{
	class ExarBuffer;

	class ExarBufferView {
		using iterator = std::span<const u8>::iterator;
		using value_type = std::span<const u8>::value_type;
	public:
		ExarBufferView(const u8* pData, size_t pSize)
			: mData(pData, pSize)
			, mCursor(0)
		{
		}

		ExarBufferView(const ExarBuffer& pBuffer);

		inline iterator begin() const noexcept { return mData.begin(); }
		inline iterator end() const noexcept { return mData.end(); }
		inline iterator current() const noexcept { return mData.begin() + mCursor; }
		inline bool empty() const noexcept { return mData.empty(); }
		inline size_t size() const noexcept { return mData.size(); }
		inline size_t remaining() const noexcept { return mData.size() - mCursor; }

		template<typename DataType>
		inline DataType get(size_t pOffset) const noexcept {
			if (pOffset + sizeof(DataType) > mData.size()) {
				return DataType{};
			}

			DataType lValue;
			std::memcpy(&lValue, mData.data() + pOffset, sizeof(DataType));
			return lValue;
		}

	private:
		std::span<const u8> mData;
		size_t mCursor = 0;
	};
}

#endif // !EXAR_BUFFER_VIEW_HPP