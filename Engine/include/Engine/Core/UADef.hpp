#ifndef UA_DEF_HPP
#define UA_DEF_HPP

#define DECLARE_UAE_HANDLE(name) typedef struct name##_ *name;

namespace UnasciiEngine {
	DECLARE_UAE_HANDLE(Handle);
}

#endif // UA_DEF_HPP
