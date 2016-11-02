#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

// redefine c++ keywords that are not recognized when using certain language extensions
#define and &&
#define or ||
#define not !

// variadic iteration macro
#define for_each_variadic(expression) auto initializer_list = { 0, (static_cast<void>(expression), 0) ... }

namespace tenjix {

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

	template <class Type>
	using Function = std::function<Type>;

	template <class First, class Second>
	using Pair = std::pair<First, Second>;
	template <class Entry, class Allocator = std::allocator<Entry>>
	using Lot = std::vector<Entry, Allocator>;
	template <class Entry, class Hasher = std::hash<Entry>, class Comparator = std::equal_to<Entry>, class Allocator = std::allocator<Entry>>
	using Set = std::unordered_set<Entry, Hasher, Comparator, Allocator>;
	template <class Key, class Value, class Hasher = std::hash<Key>, class Comparator = std::equal_to<Key>, class Allocator = std::allocator<std::pair<const Key, Value>>>
	using Map = std::unordered_map<Key, Value, Hasher, Comparator, Allocator>;
	template <class Entry, class Orderer = std::less<Entry>, class Allocator = std::allocator<std::pair<Entry>>>
	using OrderedSet = std::set<Entry, Orderer, Allocator>;
	template <class Key, class Value, class Orderer = std::less<Key>, class Allocator = std::allocator<std::pair<const Key, Value>>>
	using OrderedMap = std::map<Key, Value, Orderer, Allocator>;

	// determines the size/length of an array, i.e. how many entries of "Type" it can take
	template <typename Type, uint N>
	uint size_of(const Type(&array)[N]) { return N; }

	// determines the bytesize of the content of a vector
	template<typename Type>
	uint size_of(const typename Lot<Type>& vec) {
		return sizeof(Type) * vec.size();
	}

	// calculates a combined hash for multiple hashable objects/values
	template <typename... Types>
	inline uint hash_combined(Types... hashable) {
		uint hash = 0;
		for_each_variadic(hash ^= std::hash<Types>()(hashable) + 0x9e3779b9 + (hash << 6) + (hash >> 2));
		return hash;
	}

	struct GenericZero {

		template <class Type>
		operator Type() const {
			return Type(0);
		}

		//template <class Type>
		//bool operator==(const Type& value) const {
		//	return value == Type(0);
		//}

		//template <class Type>
		//bool operator!=(const Type& value) const {
		//	return value != Type(0);
		//}

	};

	template <class Type>
	bool operator==(const Type& value, GenericZero) {
		return value == Type(0);
	}
	template <class Type>
	bool operator!=(const Type& value, GenericZero) {
		return value != Type(0);
	}
	template <class Type>
	bool operator<(const Type& value, GenericZero) {
		return value < Type(0);
	}
	template <class Type>
	bool operator>(const Type& value, GenericZero) {
		return value > Type(0);
	}
	template <class Type>
	bool operator<=(const Type& value, GenericZero) {
		return value <= Type(0);
	}
	template <class Type>
	bool operator>=(const Type& value, GenericZero) {
		return value >= Type(0);
	}

	constexpr GenericZero Zero;

}

#ifndef TENJIX_NO_NAMESPACE_ALIAS
namespace x = tenjix;
#endif
