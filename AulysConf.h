#pragma once

// All the stuff for the guaranteed defines (e.g. compiler stuff) lives in here:
#include "AulysDefines.h"

/*  To change settings that are just #defines, comment or uncomment that specific line. */
/*  To change settings that actually have a value, read the corresponding note, because you might
 *  screw things up in a major way.  */

/**************************************************************************************************/
/*** Generic system defines ***/
/**************************************************************************************************/

// The filesystem path to the folder containing e.g. Sandbox, Aulys, build, this file.
// This is used to locate assets, mainly,
// This need a slash at the end if there's actually path defined.
// e.g. "D:/path/to/the/project/" not "D:/path/to/the/project"
//      or "" not "/"
#ifdef AU_PLATFORM_LINUX
	#define PROJECT_ROOT ""
#elif AU_PLATFORM_WINDOWS
	#define PROJECT_ROOT ""
#endif

#define AU_ENABLE_ASSERTS
#define AU_ASSERTS_ARE_EXCEPTIONS

// #define FORCE_RUN_DEFAULT_APPLICATION // In the hyperbolic branch, this is not utilised (yet)
#define ENABLE_GRAPHICS_DEBUG_OUTPUT
	// Enable output from the graphics API (OpenGL). We use glDebugMessageCallback to hook into our
	// error system, see OpenGLRendererAPI.cpp.
#define ENABLE_GRAPHICS_BLEND
	// Enable blending overlapping colors. (see GL_BLEND, OpenGLRendererAPI.cpp)

/**************************************************************************************************/
/*** Logging customization ***/
/**************************************************************************************************/
#define LOGGING_CORE_SINK_CONSOLE_MT
#define LOGGING_CORE_SINK_FILE_ROTATE

#define LOGGING_CLIENT_SINK_CONSOLE_MT
#define LOGGING_CLIENT_SINK_FILE_ROTATE

#define LOGGING_INDENT "  " // The string used to indent standard (non-header) output lines.
#define LOGGING_EMPTY_LINE "" // The string that is used instead of LOGGING_INDENT for empty lines.

#define LOGGING_PREPEND_EMPTY_LINE // Should I put a gap between the header line and the message?
#define LOGGING_POSTPEND_EMPTY_LINE // Should I put a gap after the message?

/* Logger names - hooks into %n in HEADER_FORMAT */
#define LOGGING_CORE_HEADER_NAME "Au"
#define LOGGING_CORE_LOGGER_NAME "AuLog"
#define LOGGING_CLIENT_HEADER_NAME "Cl"
#define LOGGING_CLIENT_LOGGER_NAME "ClLog"

// Here's a table of what you have access to in any log call. This includes the HEADER_FORMAT, which
// is what the purpose of this was originally, however now you have access to these in any LOG
// message.
	// String token | Defined value assigned | Examples
	// {FILE}       | __FILE__               | LOG_TRACE_LINE("This is in file {file}");
	//              |                        | --> This is in file /home/user/work/Aul/Log.h
	// {BASEFILE}   | __BASE_FILE__          | LOG_TRACE_LINE("Filename: {baseFile}")
	//              |                        | --> Filename: Aul/Log.h
	// {LINE}       | __LINE__               | LOG_TRACE_LINE("Lineno: {line}")
	//              |                        | --> Lineno: 42
	// {FUNC}       | __FUNCTION__           | LOG_TRACE_LINE("Function calling log: {func}")
	//              |                        | --> Function calling log: Foo
	// {PRETTYFUNC} | __PRETTY_FUNCTION__    | LOG_TRACE_LINE("Function calling log: {prettyFunc}")
	//              |                        | --> Function calling log: void ns::Foo(args)
	// {LOGLVL}     | Friendly name of lvl   | LOG_ERROR_LINE("This is an {LOGLVL} level msg.")
	//              |                        | --> This is an Error level msg.
	//              |                        | Trace, Info, Warn, Error
	// {LOGLVLC}    | Char name of lvl       | LOGusing_ERROR_LINE("This is an {LOGLVLC} level msg.")
	//              |                        | --> This is an E level msg.
	//              |                        | T, I, W, E

/* The HEADER_FORMAT gets injected into the HEADER_PATTERN into token %v. This is where you should
 * put your custom message. The purpose of the HEADER_PATTERN is to get access to spdlog's
 * information, such as the name of the current logger, and to adjust all of the knobs and dials
 * spdlog allows. For more info about this, look up spdlog formatting.
 * [https://github.com/gabime/spdlog/wiki/3.-Custom-formatting]
 *
 * The HEADER loggers are currently also used as the logger for one-liner messages, so make sure
 * your header is suitable for that, too. Other than that, you're free to customize to your heart's
 * content, with whatever disgusting things you can come up with. */
#define LOGGING_CORE_HEADER_PATTERN "%^[%n, %v%$"
#define LOGGING_CLIENT_HEADER_PATTERN "%^[%n, %v%$"
	// The spdlog pattern string passed to e.g.
	// mClientHeader->set_pattern(LOGGING_CLIENT_HEADER_PATTERN) in /src/Log.cpp


#include "spdlog/spdlog.h"

// Here's the place to add default log arguments of your own. We have a few defined as below, and 
// you can access them like so:
// `LOG_INFO("This function is {FUNC} located in {FILE}'s line {LINE}.")`
// To define a named argument like these ^, just add a
// `fmt::arg("NAMEOFARG", value)`
// to the following define:

#define CUSTOM_LOG_ARGS__(logLevel)\
// e.g.      `, fmt::arg("THREAD", threadID), fmt::arg("NTHREADS", threadCount)`
// ^ Pay attention to the commas here!
// You NEED to add a preceding comma and NO ending comma because this is the final argument to a
// function.
// And then this can be used like COMMON_LOG_ARGS__: `LOG_ERROR_LINE("Big trouble in {THREAD}.")`

#ifdef AU_COMPILER_MSVC
	#define COMMON_LOG_ARGS__(logLevel)\
                    fmt::arg("FILE", __FILE__), fmt::arg("BASEFILE", "[!!] BASEFILE - IS UNDEFINED ON MSVC"),\
                    fmt::arg("LINE", __LINE__),\
                    fmt::arg("FUNC", __FUNCTION__), fmt::arg("PRETTYFUNC", "[!!] PRETTYFUNC - IS UNDEFINED ON MSVC"),\
                    fmt::arg("LOGLVL", logLevel), fmt::arg("LOGLVLC", logLevel[0])\
					CUSTOM_LOG_ARGS__(logLevel)
#elif AU_COMPILER_GCC
	#define COMMON_LOG_ARGS__(logLevel)\
                    fmt::arg("FILE", __FILE__), fmt::arg("BASEFILE", __BASE_FILE__),\
                    fmt::arg("LINE", __LINE__),\
                    fmt::arg("FUNC", __FUNCTION__), fmt::arg("PRETTYFUNC", __PRETTY_FUNCTION__),\
                    fmt::arg("LOGLVL", logLevel), fmt::arg("LOGLVLC", logLevel[0])\
					CUSTOM_LOG_ARGS__(logLevel)
#elif AU_COMPILER_UNKNOWN
	#define COMMON_LOG_ARGS__(logLevel)\
                    fmt::arg("FILE", "FILE"), fmt::arg("BASEFILE", "BASE_FILE"),\
                    fmt::arg("LINE", "LINE"),\
                    fmt::arg("FUNC", "FUNCTION"), fmt::arg("PRETTYFUNC", "PRETTY_FUNCTION"),\
                    fmt::arg("LOGLVL", logLevel), fmt::arg("LOGLVLC", logLevel[0])\
					CUSTOM_LOG_ARGS__(logLevel)
#endif


#ifdef AU_COMPILER_GCC
#define LOGGING_CLIENT_HEADER_FORMAT "{BASEFILE}::{LINE}, {FUNC}]:"
#define LOGGING_CORE_HEADER_FORMAT "{BASEFILE}::{LINE}, {FUNC}]:"
#elif AU_COMPILER_MSVC
#define LOGGING_CLIENT_HEADER_FORMAT "{FILE}::{LINE}, {FUNC}]:"
#define LOGGING_CORE_HEADER_FORMAT "{FILE}::{LINE}, {FUNC}]:"
#elif AU_COMPILER_UNKNOWN
#define LOGGING_CLIENT_HEADER_FORMAT "HEADER]:"
#define LOGGING_CORE_HEADER_FORMAT "HEADER]:"
#endif
	// ^ This is given to e.g. ::Aulys::Log::getCoreHeader()->trace(LOGGING_HEADER_FORMAT)
	// (see Log.h)                (and other functions in that ^ family, info, warn, and error).
	// Note: The header always starts with LOGGING_CORE_HEADER_PATTERN, which is given to 
	// spdlog to format the header loggers.

#define LOGGING_CORE_LOGGER_PATTERN "%^%v%$" // The patterns used by the standard loggers.
#define LOGGING_CLIENT_LOGGER_PATTERN "%^%v%$"

