#ifndef EXAR_HPP
#define EXAR_HPP

#ifdef _WIN32
	#ifdef EXAR_BUILD_DLL
		#define EXA_API __declspec(dllexport)
	#else
		#define EXA_API __declspec(dllimport)
	#endif
#endif

#endif // !ENGINE_HPP
