#pragma once

#include <cerrno>
#include <limits>
#include <functional>
#include <vector>

#include <Utilities/Assertions.h>

// internal

template <typename Type, int N>
static void _parse_groups(const char* text, const std::function<Type(const char*, char**)>& parse, const std::function<void(void)>& consume, Type(&values)[N]) {
	uint offset = 0;
	std::function<void(Type)> consume_group = [&consume, &values, &offset](Type value) {
		values[offset] = value;
		if (offset == N - 1) consume();
		offset = ++offset % N;
	};
	parse_singles(text, parse, consume_group);
	runtime_assert(offset == 0, "error while parsing text for space sparated values in groups of ", N, ": number of elements was not a multiple of the size of a group. input was: \"", text, "\"")
}

// generic

template <typename Type>
void parse_singles(const char* text, const std::function<Type(const char*, char**)>& parse, const std::function<void(Type)>& consume) {
	char* next = "";
	for (Type value = parse(text, &next); text != next; value = parse(text, &next)) {
		runtime_assert(errno != ERANGE, "range error while parsing text for space sparated values. input was: \"", text, "\"");
		consume(value);
		text = next;
	}
}

template <typename Type>
inline void parse_tuples(const char* text, const std::function<Type(const char*, char**)>& parse, const std::function<void(Type, Type)>& consume) {
	Type values[2];
	std::function<void(void)> unpack = [&consume, &values]() { consume(values[0], values[1]); };
	_parse_groups(text, parse, unpack, values);
}

template <typename Type>
inline void parse_triples(const char* text, const std::function<Type(const char*, char**)>& parse, const std::function<void(Type, Type, Type)>& consume) {
	Type values[3];
	std::function<void(void)> unpack = [&consume, &values]() { consume(values[0], values[1], values[2]); };
	_parse_groups(text, parse, unpack, values);
}

template <typename Type>
inline void parse_quadruples(const char* text, const std::function<Type(const char*, char**)>& parse, const std::function<void(Type, Type, Type, Type)>& consume) {
	Type values[4];
	std::function<void(void)> unpack = [&consume, &values]() { consume(values[0], values[1], values[2], values[3]); };
	_parse_groups(text, parse, unpack, values);
}

/// floats

// parses "text" for space separated float values and invokes "consume" for each value
inline void parse_floats(const char* text, const std::function<void(float)>& consume) {
	std::function<float(const char*, char**)> parse = std::strtof;
	parse_singles(text, parse, consume);
}

// parses "text" for space separated float values and invokes "consume" for every two values
inline void parse_float_tuples(const char* text, const std::function<void(float, float)>& consume) {
	std::function<float(const char*, char**)> parse = std::strtof;
	parse_tuples(text, parse, consume);
}

// parses "text" for space separated float values and invokes "consume" for every three values
inline void parse_float_triples(const char* text, const std::function<void(float, float, float)>& consume) {
	std::function<float(const char*, char**)> parse = std::strtof;
	parse_triples(text, parse, consume);
}

// parses "text" for space separated float values and invokes "consume" for every four values
inline void parse_float_quadruples(const char* text, const std::function<void(float, float, float, float)>& consume) {
	std::function<float(const char*, char**)> parse = std::strtof;
	parse_quadruples(text, parse, consume);
}

// parses "text" for space separated float values and returns them as vector
// (initially reserves vector space for "expected_values")
inline std::vector<float> parse_floats(const char* text, unsigned expected_values = 1) {
	std::vector<float> vector;
	vector.reserve(expected_values);
	parse_floats(text, [&vector](float value) { vector.push_back(value); });
	return vector;
}

/// doubles

// parses "text" for space separated double values and invokes "consume" for each value
inline void parse_doubles(const char* text, const std::function<void(double)>& consume) {
	std::function<double(const char*, char**)> parse = std::strtod;
	parse_singles(text, parse, consume);
}

// parses "text" for space separated double values and invokes "consume" for every two values
inline void parse_double_tuples(const char* text, const std::function<void(double, double)>& consume) {
	std::function<double(const char*, char**)> parse = std::strtod;
	parse_tuples(text, parse, consume);
}

// parses "text" for space separated double values and invokes "consume" for every three values
inline void parse_double_triples(const char* text, const std::function<void(double, double, double)>& consume) {
	std::function<double(const char*, char**)> parse = std::strtod;
	parse_triples(text, parse, consume);
}

// parses "text" for space separated double values and invokes "consume" for every four values
inline void parse_double_quadruples(const char* text, const std::function<void(double, double, double, double)>& consume) {
	std::function<double(const char*, char**)> parse = std::strtod;
	parse_quadruples(text, parse, consume);
}

// parses "text" for space separated double values and returns them as vector
// (initially reserves vector space for "expected_values")
inline std::vector<double> parse_doubles(const char* text, unsigned expected_values = 1) {
	std::vector<double> vector;
	vector.reserve(expected_values);
	parse_doubles(text, [&vector](double value) { vector.push_back(value); });
	return vector;
}

/// signed integers

// parses "text" for space separated signed integer values and invokes "consume" for each value
inline void parse_signed_integers(const char* text, const std::function<void(long)>& consume, unsigned base = 0) {
	std::function<long(const char*, char**)> parse = [&base](const char* text, char** next) { return std::strtol(text, next, base); };
	parse_singles(text, parse, consume);
}

// parses "text" for space separated signed integer values and invokes "consume" for every two values
inline void parse_signed_integer_tuples(const char* text, const std::function<void(long, long)>& consume, unsigned base = 0) {
	std::function<long(const char*, char**)> parse = [&base](const char* text, char** next) { return std::strtol(text, next, base); };
	parse_tuples(text, parse, consume);
}

// parses "text" for space separated signed integer values and invokes "consume" for every three values
inline void parse_signed_integer_triples(const char* text, const std::function<void(long, long, long)>& consume, unsigned base = 0) {
	std::function<long(const char*, char**)> parse = [&base](const char* text, char** next) { return std::strtol(text, next, base); };
	parse_triples(text, parse, consume);
}

// parses "text" for space separated signed integer values and invokes "consume" for every four values
inline void parse_signed_integer_quadruples(const char* text, const std::function<void(long, long, long, long)>& consume, unsigned base = 0) {
	std::function<long(const char*, char**)> parse = [&base](const char* text, char** next) { return std::strtol(text, next, base); };
	parse_quadruples(text, parse, consume);
}

// parses "text" for space separated signed integer values and returns them as vector
// (initially reserves vector space for "expected_values")
inline std::vector<long> parse_signed_integers(const char* text, unsigned expected_values = 1) {
	std::vector<long> vector;
	vector.reserve(expected_values);
	parse_signed_integers(text, [&vector](long value) { vector.push_back(value); });
	return vector;
}

/// unsigned integers

// parses "text" for space separated unsigned integer values and invokes "consume" for each value
inline void parse_unsigned_integers(const char* text, const std::function<void(unsigned long)>& consume, unsigned base = 0) {
	std::function<unsigned long(const char*, char**)> parse = [&base](const char* text, char** next) { return std::strtoul(text, next, base); };
	parse_singles(text, parse, consume);
}

// parses "text" for space separated unsigned integer values and invokes "consume" for every two values
inline void parse_unsigned_integer_tuples(const char* text, const std::function<void(unsigned long, unsigned long)>& consume, unsigned base = 0) {
	std::function<unsigned long(const char*, char**)> parse = [&base](const char* text, char** next) { return std::strtoul(text, next, base); };
	parse_tuples(text, parse, consume);
}

// parses "text" for space separated unsigned integer values and invokes "consume" for every three values
inline void parse_unsigned_integer_triples(const char* text, const std::function<void(unsigned long, unsigned long, unsigned long)>& consume, unsigned base = 0) {
	std::function<unsigned long(const char*, char**)> parse = [&base](const char* text, char** next) { return std::strtoul(text, next, base); };
	parse_triples(text, parse, consume);
}

// parses "text" for space separated unsigned integer values and invokes "consume" for every four values
inline void parse_unsigned_integer_quadruples(const char* text, const std::function<void(unsigned long, unsigned long, unsigned long, unsigned long)>& consume, unsigned base = 0) {
	std::function<unsigned long(const char*, char**)> parse = [&base](const char* text, char** next) { return std::strtoul(text, next, base); };
	parse_quadruples(text, parse, consume);
}

// parses "text" for space separated unsigned integer values and returns them as vector
// (initially reserves vector space for "expected_values")
inline std::vector<unsigned long> parse_unsigned_integers(const char* text, uint expected_values = 1) {
	std::vector<unsigned long> vector;
	vector.reserve(expected_values);
	parse_unsigned_integers(text, [&vector](unsigned long value) { vector.push_back(value); });
	return vector;
}
