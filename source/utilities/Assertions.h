#pragma once

#include <iostream>
#include <stdexcept>

#include <utilities/Strings.h>

//#define _RUNTIME_ASSERTIONS_DISABLED // to disable assertions

#if defined(_RUNTIME_ASSERTIONS_DISABLED)
#	define runtime_assert(condition, ...) static_cast<void>(condition)
#else
#	define runtime_assert(condition, ...) \
		if (not (condition)) { \
			const String& message = stringify("Assertion failed: ", __VA_ARGS__); \
			std::cout << __FILE__ << "(" << __LINE__ << "):" << __FUNCTION__ << "() : " << std::endl << message << std::endl << "Expression: " << #condition << std::endl; \
			throw std::runtime_error(message); \
		}
#endif
