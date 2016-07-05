#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>

// redefine c++ keywords not recognized by some language extensions
#define and &&
#define or ||
#define not !

// variadic iteration macro
#define for_each_variadic(expression) auto initializer_list = { 0, (static_cast<void>(expression), 0) ... }

// define important constants
constexpr double Pi = 3.14159265358979323846;
constexpr double Pi_Inverse = 0.318309886183790671538;
constexpr double Pi_Half = 1.57079632679489661923;
constexpr double Sqrt_2 = 1.41421356237309504880;
constexpr double Sqrt_2_Inverse = 0.707106781186547524401;
constexpr double Sqrt_3 = 1.73205080756887729353;
constexpr double Sqrt_3_Inverse = 0.577350269189625764509;

// redefine standard types
using int8 = std::int8_t;
using int16 = std::int16_t;
using int32 = std::int32_t;
using int64 = std::int64_t;

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

using uint = std::size_t;

using String = std::string;

template <class Type>
using linked = std::weak_ptr<Type>;
template <class Type>
using shared = std::shared_ptr<Type>;
template <class Type>
using unique = std::unique_ptr<Type>;

// determines the size/length of an array, i.e. how many entries of "Type" it can take
template <typename Type, uint N>
uint size_of(const Type(&array)[N]) { return N; }

// returns the maximum of two given values using the ">" operator
template <typename Type>
inline const Type& maximum(const Type& a, const Type& b) { return a > b ? a : b; }

// returns the minimum of two given values using the "<" operator
template <typename Type>
inline const Type& minimum(const Type& a, const Type& b) { return a < b ? a : b; }

// calculates a combined hash for multiple hashable objects/values
template <typename... Types>
inline std::size_t hash_combined(Types... hashable) {
	std::size_t hash = 0;
	for_each_variadic(hash ^= std::hash<Types>()(hashable) + 0x9e3779b9 + (hash << 6) + (hash >> 2));
	return hash;
}

// calculates the signum function (negative => -1, zero => 0, positive => +1)
template <typename Type>
int signum(Type value) {
	return (Type(0) < value) - (value < Type(0));
}
