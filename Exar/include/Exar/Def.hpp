#ifndef EXAR_DEF_HPP
#define EXAR_DEF_HPP

#define DECLARE_EXAR_HANDLE(name) typedef struct name##_ *name;

namespace Exar {
	DECLARE_EXAR_HANDLE(MemoryHandle);
}

#endif // EXAR_DEF_HPP
