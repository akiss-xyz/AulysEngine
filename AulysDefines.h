#pragma once

// Guaranteed defines:

#ifdef _MSC_VER
	#define AU_COMPILER_MSVC 1
#elif __GNUC__
	#define AU_COMPILER_GCC 1
#else
	#define AU_COMPILER_UNKNOWN 1
#endif
