#pragma once

#include <functional>

#include <Utilities/Assertions.h>

template <typename Type>
class Optional {

	Type* _value;

public:

	explicit Optional() : _value(nullptr) {}
	explicit Optional(Type& value) : _value(&value) {}

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

	//operator Type&() const {
	//	return value();
	//}

};