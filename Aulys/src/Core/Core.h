#pragma once

#include "pch.h"

#include "Log/Log.h"

#ifdef AU_PLATFORM_LINUX
	#define PLATFORM_EOL_CHAR '\n'
	#define PLATFORM_EOL_CHAR_PRINT_FRIENDLY "\\n"
#endif

#ifdef AU_PLATFORM_WINDOWS
	#define PLATFORM_EOL_CHAR '\r\n'
	#define PLATFORM_EOL_CHAR_PRINT_FRIENDLY "\\r\\n"
#endif

// Usage: AU_ASSERT(exprYouWantToBeTrue, ...)
// e.g. AU_ASSERT(var0==var1, "var0 isn't equal to var1", var0={0}, var1={1}", var0, var1)
#ifdef AU_ASSERTS_ARE_EXCEPTIONS
class AssertException : public std::exception {
public:
	AssertException(const std::string& s) : msg(s) {};

	virtual const char* what() const noexcept override {
		return msg.c_str();
	}
	
	std::string msg;
private:

};
  #define AU_ASSERT(x, ...) { if(!(x)) { LOG_ERROR("--- ASSERTION FAILED:"); LOG_ERROR(__VA_ARGS__); throw AssertException(fmt::format(__VA_ARGS__)); } }

  #define AU_CORE_ASSERT(x, ...) { if(!(x)) { AU_LOG_ERROR("--- ASSERTION FAILED:"); AU_LOG_ERROR(__VA_ARGS__); throw AssertException(fmt::format(__VA_ARGS__)); } }

#elif AU_ENABLE_ASSERTS
  #define AU_ASSERT(x, ...) { if(!(x)) { LOG_ERROR("--- ASSERTION FAILED:"); LOG_ERROR(__VA_ARGS__); assert(x); } }

  #define AU_CORE_ASSERT(x, ...) { if(!(x)) { AU_LOG_ERROR("--- ASSERTION FAILED:"); AU_LOG_ERROR(__VA_ARGS__); assert(x); } }
#else
  #define AU_ASSERT(x, ...)
  #define AU_CORE_ASSERT(x, ...)
#endif

namespace Aulys
{
	// These are our future-proofing measures - we'll replace all
	// the instances of std::[]_ptr with these, so that in the future we can easily change them to
	// what we need and don't need.
	template<typename T>
	using Uni = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

	class Path
	{
	public:
		static std::string replaceLinuxSlashToWinSlash(const std::string& s);

		static std::string replaceWinSlashToLinuxSlash(const std::string& s);

#ifdef AU_PLATFORM_WINDOWS
		Path(const std::string& relativePathFromProjectRoot, bool raw = false) {
			if (raw) {
				mPath = Path::replaceLinuxSlashToWinSlash(relativePathFromProjectRoot);
			}
			else {
				auto path = Path::replaceLinuxSlashToWinSlash(relativePathFromProjectRoot);
				std::stringstream ss; ss << PROJECT_ROOT;
				path[0] == '\\' ?
					ss << path.substr(1) : // Remove the slash
					ss << path; // We're fine
				mPath = ss.str();
			}
		};
#else
		Path(const std::string& relativePathFromProjectRoot, bool raw = false) {
			if(raw) {
				mPath = Path::replaceWinSlashToLinuxSlash(relativePathFromProjectRoot);
			}
			else {
				auto path = Path::replaceWinSlashToLinuxSlash(relativePathFromProjectRoot);
				std::stringstream ss; ss << PROJECT_ROOT;
				path[0] == '/' ?
					ss << path.substr(1) : // Remove the first slash.
					ss << path; // Otherwise just use the path.
				mPath = ss.str();
			}
		};
#endif
		const char* get() const noexcept { return mPath.data(); };
		const std::string* gets() const noexcept { return &mPath; };

	private:
		std::string mPath;
	}; // class Path
};
