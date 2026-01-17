#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>

#ifdef _WIN32
#include <Windows.h>
#endif // !_WIN32


namespace UnasciiEngine {
	using u8 = uint8_t;
	using u16 = uint16_t;
	using u32 = uint32_t;
	using u64 = uint64_t;

	using i8 = int8_t;
	using i16 = int16_t;
	using i32 = int32_t;
	using i64 = int64_t;

	using f32 = float;
	using f64 = double;

	using String = std::string;
	using StringC = const char*;
	using StringV = std::string_view;

#ifdef _WIN32
	using StringW = std::wstring;
	using StringWC = const wchar_t*;
#endif // !_WIN32
}

#endif // !TYPES_HPP

