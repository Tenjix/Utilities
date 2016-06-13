#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

// redefine c++ keywords not recognized by some language extensions
#define and &&
#define or ||
#define not !

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

template <class T>
using linked = std::weak_ptr<T>;
template <class T>
using shared = std::shared_ptr<T>;
template <class T>
using unique = std::unique_ptr<T>;

// determines the size/length of an array, i.e. how many entries of "Type" it can take
template <typename Type, uint N>
uint size_of(const Type(&array)[N]) { return N; }

// returns the maximum of two given values using the ">" operator
template <typename Type>
inline const Type& maximum(const Type& a, const Type& b) { return a > b ? a : b; }

// returns the minimum of two given values using the "<" operator
template <typename Type>
inline const Type& minimum(const Type& a, const Type& b) { return a < b ? a : b; }
