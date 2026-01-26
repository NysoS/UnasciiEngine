#ifndef RHI_DEF_HPP
#define RHI_DEF_HPP

#define DECLARE_RHI_HANDLE(name) typedef struct name##_ *name;

namespace UnasciiEngine::RHI {
	DECLARE_RHI_HANDLE(MemHandle);
	DECLARE_RHI_HANDLE(RHIHandle);
}

#endif // !RHI_DEF_HPP

