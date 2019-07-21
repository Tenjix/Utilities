#pragma once

#include <iostream>

///  Logging Utility

/// use print(...) for normal logging
/// use error(...) for error logging
/// use debug(...) for debug logging (requires _DEBUG to be defined)
/// use trace(...) for trace logging (requires _DEBUG and _LOGGING_TRACE_ENABLED to be defined)

//#define _LOGGING_DISABLED // to disable logging
//#define _LOGGING_ERROR_DISABLED // to disable error logging
//#define _LOGGING_PRINT_DISABLED // to disable print logging
//#define _LOGGING_DEBUG_DISABLED // to disable debug logging

//#define _LOGGING_DEBUG_RELEASE // to enable debug logging for release
//#define _LOGGING_TRACE_ENABLED // to enable trace logging
//#define _LOGGING_DETAILED // to enable detailed logging

#define __ERROR_PREFIX__ "[E] "
#define __PRINT_PREFIX__ "[ ] "
#define __DEBUG_PREFIX__ "[D] "
#define __TRACE_PREFIX__ "[T] "

#ifndef _LOGGING_OUTPUT_STREAM
#	define _LOGGING_OUTPUT_STREAM std::cout
#endif
#ifndef _LOGGING_ERROR_STREAM
#	define _LOGGING_ERROR_STREAM std::cerr
#endif

template <typename... Arguments>
inline void _print(std::ostream& stream, const char* file, const int line, const char* function, const Arguments&... arguments) {
	#ifdef _LOGGING_DETAILED
	stream << file << "(" << line << "):" << function << "() : ";
	#endif
	auto list = { 0, ((void)(stream << arguments), 0) ... };
	stream << std::endl;
}

#if defined(_LOGGING_DISABLED) || defined(_LOGGING_ERROR_DISABLED)
#	define error(...)
#else
#	define error(...) _print(_LOGGING_ERROR_STREAM, __FILE__, __LINE__, __FUNCTION__, __ERROR_PREFIX__, __VA_ARGS__)
#endif

#if defined(_LOGGING_DISABLED) || defined(_LOGGING_PRINT_DISABLED)
#	define print(...)
#else
#	define print(...) _print(_LOGGING_OUTPUT_STREAM, __FILE__, __LINE__, __FUNCTION__, __PRINT_PREFIX__, __VA_ARGS__)
#endif

#if defined(_LOGGING_DISABLED) || defined(_LOGGING_DEBUG_DISABLED) || !defined(_DEBUG) && !defined(_LOGGING_DEBUG_RELEASE)
#	define debug(...)
#else
#	define debug(...) _print(_LOGGING_OUTPUT_STREAM, __FILE__, __LINE__, __FUNCTION__, __DEBUG_PREFIX__, __VA_ARGS__)
#endif

#if defined(_LOGGING_DISABLED) || !defined(_LOGGING_TRACE_ENABLED) || !defined(_DEBUG) && !defined(_LOGGING_DEBUG_RELEASE)
#	define trace(...)
#else
#	define trace(...) _print(_LOGGING_OUTPUT_STREAM, __FILE__, __LINE__, __FUNCTION__, __TRACE_PREFIX__, __VA_ARGS__)
#endif
