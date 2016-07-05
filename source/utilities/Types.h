#pragma once

#include <iostream>
#include <sstream>
#include <typeindex>
#include <unordered_set>

#include <utilities/Standard.h>

class Type {

	const type_info& info;

public:

	Type(const type_info& info) noexcept : info(info) {}

	uint hash_code() const noexcept {
		return info.hash_code();
	}

	String name() const noexcept {
		String name = info.name();
		auto domain_pos = name.find_last_of("::");
		if (domain_pos == String::npos) {
			auto space_pos = name.find_last_of(' ');
			return space_pos == String::npos ? name : name.substr(space_pos + 1);
		}
		return name.substr(domain_pos + 1);
	}

	bool operator==(const Type& other) const noexcept {
		return (info == other.info);
	}

	bool operator!=(const Type& other) const noexcept {
		return (not (*this == other));
	}

	bool operator<(const Type& other) const noexcept {
		return (info.before(other.info));
	}

	bool operator>=(const Type& other) const noexcept {
		return (not (*this < other));
	}

	bool operator>(const Type& other) const noexcept {
		return (other < *this);
	}

	bool operator<=(const Type& other) const noexcept {
		return (not (other < *this));
	}

	friend std::ostream& operator<<(std::ostream& output, const Type& type) {
		return (output << type.name());
	}

	template <typename TYPE>
	static Type of() {
		return Type(typeid(TYPE));
	}

};

template <typename TYPE>
Type type_of() {
	return Type::of<TYPE>();
}

namespace std {

	template<>
	struct hash<Type> {
		size_t operator()(const Type& type) const noexcept {
			return type.hash_code();
		}
	};

}

class Types {

	using Set = std::unordered_set<Type>;

	Set set;

public:

	Types() = default;

	Types(std::initializer_list<Type> types) : set{ types } {}

	Types& insert(Type type) {
		set.insert(type);
		return *this;
	}

	template <typename... TYPES>
	Types& insert() {
		for_each_variadic(set.insert(typeid(TYPES)));
		return *this;
	}

	template <typename... TYPES>
	Types& erase() {
		for_each_variadic(set.erase(typeid(TYPES)));
		return *this;
	}

	template <typename... TYPES>
	bool contain() const {
		return contain(Types{ typeid(TYPES)... });
	}

	bool contain(Types types) const {
		for (Type type : types.set) {
			if (not contain(type)) return false;
		}
		return true;
	}

	bool contain(Type type) const {
		return set.find(type) != set.end();
	}

	bool empty() const { return set.empty(); }

	uint count() const { return set.size(); }

	Set::const_iterator begin() const { return set.begin(); }
	Set::const_iterator end() const { return set.end(); }

	friend std::ostream& operator<<(std::ostream& output, const Types& types) {
		bool first = true;
		for (Type type : types.set) {
			if (first) first = false;
			else output << ", ";
			output << type;
		}
		return output;
	}

	template <typename... TYPES>
	static Types of() {
		return Types{ typeid(TYPES)... };
	}

};

template <typename... TYPES>
Types types_of() {
	return Types::of<TYPES...>();
};
