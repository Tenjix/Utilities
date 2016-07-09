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

// calculates a combined hash for multiple hashable objects/values
template <typename... Types>
inline std::size_t hash_combined(Types... hashable) {
	std::size_t hash = 0;
	for_each_variadic(hash ^= std::hash<Types>()(hashable) + 0x9e3779b9 + (hash << 6) + (hash >> 2));
	return hash;
}
