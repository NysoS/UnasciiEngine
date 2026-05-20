#ifndef EXAR_TYPES_HPP
#define EXAR_TYPES_HPP

#include <string>

namespace Exar {
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

#if WIN32
#include <Windows.h>
#define SysAloc(Add, Size, AllocType, Protect) VirtualAlloc(Add, Size, AllocType, Protect)
#define SysFree(Handle, Size) VirtualFree(Handle, Size, MEM_RELEASE)
#else
#define SysFree()
#endif
}

#endif // !EXAR_TYPES_HPP

