#ifndef ENGINE_HPP
#define ENGINE_HPP

#ifdef _WIN32
	#ifdef UNASCII_BUILD_DLL
		#define UAE_API __declspec(dllexport)
	#else
		#define UAE_API __declspec(dllimport)
	#endif
#endif

#endif // !ENGINE_HPP
