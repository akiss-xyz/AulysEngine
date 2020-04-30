#pragma once

#include "LogImpl.h"

/* This file contains useful macros that you should use for logging/debugging purposes.  */

// You don't have to use these macros. You should, but you don't have to.
// If you just want to log messages without any headers or anything, 
	// You're welcome to use Aulys::Log::mClientLogset->logger, which is the Aulys-managed
    // spdlog logger - if you use just this, e.g.
	//      `Log::mClientLogset->logger->info("My variable has value {0}", var);`
	// then no extra formatting will be added to the stdout sink (as in, to the terminal).
	// Or manage your own loggers.

// These macros implement a neat system of tokens that'll automatically format and place
// headers with detailed information about your log message (e.g. the function that sent
// the log message, source file and line, etc.).

// You can customize these headers and such by reading through the documentation here,
// in AulysConf.h and the spdlog docs (https://github.com/gabime/spdlog/wiki).

	/* Available logging tokens */
// String token | Defined value assigned | Examples
// {FILE}       | __FILE__               | LOG_TRACE_LINE("This is in file {file}");
//              |                        | --> This is in file /home/user/work/Aul/Log.h
// {BASEFILE}   | __BASE_FILE__          | LOG_TRACE_LINE("Filename: {baseFile}")
//              |                        | --> Filename: Aul/Log.h
// {LINE}       | __LINE__               | LOG_TRACE_LINE("Lineno: {line}")
//              |                        | --> Lineno: 42
// {FUNC}       | __FUNCTION__           | LOG_TRACE_LINE("Function calling log: {func}")
//              |                        | --> Function calling log: void ns::Foo(args)
// {PRETTYFUNC} | FUCKYOU    | LOG_TRACE_LINE("Function calling log: {prettyFunc}")
//              |                        | --> Function calling log: Foo
// {LOGLVL}     | Friendly name of lvl   | LOG_ERROR_LINE("This is an {LOGLVL} level msg.")
//              |                        | --> This is an Error level msg.
//              |                        | Trace, Info, Warn, Error
// {LOGLVLC}    | Char name of lvl       | LOG_ERROR_LINE("This is an {LOGLVLC} level msg.")
//              |                        | --> This is an E level msg.
//              |                        | T, I, W, E

	/* Logging your own objects */
/* To log your own objects, like this:
 *     YourObjectType yourObjectInstance;
 *     LOG_TRACE("My object looks like {0}!", yourObjectInstance);
 *     
 * You have to provide a function that looks like this:
 *     inline std::ostream& operator<<(std::ostream& os, const Event& e) {
 *         return os << "A string of your making!";
 *     } */

// Core log macros
#define AU_LOG_TRACE(...)           AU_HEADER_TRACE__("", COMMON_LOG_ARGS__("Trace"));\
                                    AU_LOG_TRACE__(__VA_ARGS__, COMMON_LOG_ARGS__("Trace"))
#define AU_LOG_INFO(...)            AU_HEADER_INFO__("", COMMON_LOG_ARGS__("Info"));\
                                    AU_LOG_INFO__(__VA_ARGS__, COMMON_LOG_ARGS__("Info"))
#define AU_LOG_WARN(...)            AU_HEADER_WARN__("", COMMON_LOG_ARGS__("Warn"));\
                                    AU_LOG_WARN__(__VA_ARGS__, COMMON_LOG_ARGS__("Warn"))
#define AU_LOG_ERROR(...)           AU_HEADER_ERROR__("", COMMON_LOG_ARGS__("Error"));\
                                    AU_LOG_ERROR__(__VA_ARGS__, COMMON_LOG_ARGS__("Error"))

#define ALOGT(...)                  AU_LOG_TRACE(__VA_ARGS__)
#define ALOGI(...)                  AU_LOG_INFO(__VA_ARGS__)
#define ALOGW(...)                  AU_LOG_WARN(__VA_ARGS__)
#define ALOGE(...)                  AU_LOG_ERROR(__VA_ARGS__)

#define ALT(...)                    AU_LOG_TRACE(__VA_ARGS__)
#define ALI(...)                    AU_LOG_INFO(__VA_ARGS__)
#define ALW(...)                    AU_LOG_WARN(__VA_ARGS__)
#define ALE(...)                    AU_LOG_ERROR(__VA_ARGS__)

#define AU_LOG_TRACE_LINE(...)      AU_HEADER_TRACE__(__VA_ARGS__, COMMON_LOG_ARGS__("Trace"))
#define AU_LOG_INFO_LINE(...)       AU_HEADER_INFO__(__VA_ARGS__, COMMON_LOG_ARGS__("Trace"))
#define AU_LOG_WARN_LINE(...)       AU_HEADER_WARN__(__VA_ARGS__, COMMON_LOG_ARGS__("Trace"))
#define AU_LOG_ERROR_LINE(...)      AU_HEADER_ERROR__(__VA_ARGS__, COMMON_LOG_ARGS__("Trace"))

#define ALOGTL(...)                 AU_LOG_TRACE_LINE(__VA_ARGS__);
#define ALOGIL(...)                 AU_LOG_INFO_LINE(__VA_ARGS__);
#define ALOGWL(...)                 AU_LOG_WARN_LINE(__VA_ARGS__);
#define ALOGEL(...)                 AU_LOG_ERROR_LINE(__VA_ARGS__);

#define ALTL(...)                   AU_LOG_TRACE_LINE(__VA_ARGS__)
#define ALIL(...)                   AU_LOG_INFO_LINE(__VA_ARGS__)
#define ALWL(...)                   AU_LOG_WARN_LINE(__VA_ARGS__)
#define ALEL(...)                   AU_LOG_ERROR_LINE(__VA_ARGS__)

// Client log macros

#define LOG_TRACE(...)              HEADER_TRACE__("", COMMON_LOG_ARGS__("Trace"));\
                                    LOG_TRACE__(__VA_ARGS__, COMMON_LOG_ARGS__("Trace"))
#define LOG_INFO(...)               HEADER_INFO__("", COMMON_LOG_ARGS__("Info"));\
                                    LOG_INFO__(__VA_ARGS__, COMMON_LOG_ARGS__("Info"))
#define LOG_WARN(...)               HEADER_WARN__("", COMMON_LOG_ARGS__("Warn"));\
                                    LOG_WARN__(__VA_ARGS__, COMMON_LOG_ARGS__("Warn"))
#define LOG_ERROR(...)              HEADER_ERROR__("", COMMON_LOG_ARGS__("Error"));\
                                    LOG_ERROR__(__VA_ARGS__, COMMON_LOG_ARGS__("Error"))

#define LOGT(...)                   LOG_TRACE(__VA_ARGS__)
#define LOGI(...)                   LOG_INFO(__VA_ARGS__)
#define LOGW(...)                   LOG_WARN(__VA_ARGS__)
#define LOGE(...)                   LOG_ERROR(__VA_ARGS__)

#define LT(...)                     LOG_TRACE(__VA_ARGS__)
#define LI(...)                     LOG_INFO(__VA_ARGS__)
#define LW(...)                     LOG_WARN(__VA_ARGS__)
#define LE(...)                     LOG_ERROR(__VA_ARGS__)

#define LOG_TRACE_LINE(...)         HEADER_TRACE__(__VA_ARGS__, COMMON_LOG_ARGS__("Trace"))
#define LOG_INFO_LINE(...)          HEADER_INFO__(__VA_ARGS__, COMMON_LOG_ARGS__("Info"))
#define LOG_WARN_LINE(...)          HEADER_WARN__(__VA_ARGS__, COMMON_LOG_ARGS__("Warn"))
#define LOG_ERROR_LINE(...)         HEADER_ERROR__(__VA_ARGS__, COMMON_LOG_ARGS__("Error"))

#define LOGTL(...)                  LOG_TRACE_LINE(__VA_ARGS__);
#define LOGIL(...)                  LOG_INFO_LINE(__VA_ARGS__);
#define LOGWL(...)                  LOG_WARN_LINE(__VA_ARGS__);
#define LOGEL(...)                  LOG_ERROR_LINE(__VA_ARGS__);

#define LTL(...)                    LOG_TRACE_LINE(__VA_ARGS__)
#define LIL(...)                    LOG_INFO_LINE(__VA_ARGS__)
#define LWL(...)                    LOG_WARN_LINE(__VA_ARGS__)
#define LEL(...)                    LOG_ERROR_LINE(__VA_ARGS__)
