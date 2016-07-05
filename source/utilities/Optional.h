#pragma once

#include <functional>

#include <utilities/Assertions.h>

// Optional value using a shared pointer
// (a "non-existing" value can be constructed implicitly from a nullptr or by using the default constructor)
template <typename Type>
class Optional {

	shared<Type> _value;

public:

	// default constructor and implicit conversion from nullptr to non-existing optional value
	Optional(const shared<Type>& value = nullptr) : _value(value) {}
	Optional(const Type& value) : _value(std::make_shared<Type>(value)) {}
	Optional(Type&& value) : _value(std::make_shared<Type>(std::move(value))) {}

	bool exists() const {
		return _value != nullptr;
	}

	Type& value() const {
		runtime_assert(exists(), "optional value has to exist to retrive it");
		return *_value;
	}

	Type& value_or(Type& other) const {
		if (exists()) return *_value;
		return other;
	}

	const Optional<Type>& then(std::function<void(Type&)> operation) const {
		if (exists()) operation(*_value);
		return *this;
	}

	const Optional<Type>& otherwise(std::function<void()> operation) const {
		if (not exists()) operation();
		return *this;
	}

	operator bool() const {
		return exists();
	}

	operator Type&() const {
		return value();
	}

};

// Optional value using a value copy and an existance flag
// (a "non-existing" value can be constructed implicitly from a nullptr or by using the default constructor)
template <typename Type>
class Potential {

	Type _value;
	bool _exists;

public:

	// default constructor and implicit conversion from nullptr to non-existing optional value
	Potential(void* null_pointer = nullptr) : _exists(false) {
		runtime_assert(null_pointer == nullptr, "optional value can't be instantiated with a non-null-pointer");
	}
	Potential(const Type& value) : _value(value), _exists(true) {}
	Potential(Type&& value) : _value(std::move(value)), _exists(true) {}

	bool exists() const {
		return _exists;
	}

	const Type& value() const {
		runtime_assert(exists(), "optional value has to exist to retrive it");
		return _value;
	}

	const Type& value_or(Type& other) const {
		if (exists()) return _value;
		return other;
	}

	const Potential<Type>& then(std::function<void(const Type&)> operation) const {
		if (exists()) operation(_value);
		return *this;
	}

	const Potential<Type>& otherwise(std::function<void()> operation) const {
		if (not exists()) operation();
		return *this;
	}

	operator bool() const {
		return exists();
	}

	operator Type&() const {
		return value();
	}

};