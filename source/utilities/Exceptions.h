#pragma once

#include <exception>

#include <utilities/Logging.h>
#include <utilities/Strings.h>

#define throw_(...) { error(__VA_ARGS__); throw RuntimeException(__VA_ARGS__); }

class RuntimeException : public std::exception {

public:

	template <typename... Arguments>
	explicit RuntimeException(Arguments&&... arguments) : std::exception(stringify(std::forward<Arguments>(arguments)...).c_str()) {}

	virtual ~RuntimeException() = default;

};