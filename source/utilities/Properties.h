#pragma once

#include <functional>
#include <iostream>
#include <utility>

#include <Utilities/Assertions.h>

/// Utility templates for emulating properties

/// Each property has three sets of redundant accessors:
/// 1. function call syntax
/// 2. get() and set() functions
/// 3. overloaded operator =

template<class Type>
class Assignment {

	std::function<void(Type&)> assign_to;

public:

	Assignment(const Type& value) : assign_to([&value](Type& target) { target = value; }) {}
	Assignment(Type&& value) : assign_to([&value](Type& target) { target = std::move(value); }) {}
	Assignment(Assignment&&) = default;

	const Type& to(Type& target) const { assign_to(target); return target; }

};

// a read-write property with data store and automatically generated get/set functions
// this is what C++/CLI calls a trivial scalar property
template <class Type, class Object>
class Property {

	Type data;
	Object* object;

public:

	Property(const Type& value) : data(value), object(nullptr) {}
	Property(Type&& value) : data(std::move(value)), object(nullptr) {}
	Property(Object* owner_object = nullptr) : data(), object(owner_object) {}

	Property(const Property&) = delete;
	Property(Property&&) = delete;

	Property& operator=(const Property&) = delete;
	Property& operator=(Property&&) = delete;

	// initializer to specify the owner
	void operator()(Object* owner_object) {
		runtime_assert(object == nullptr, "property has already been initialized");
		runtime_assert(owner_object != nullptr, "owner_object must not be null");
		this->object = owner_object;
	}

	// access with function call syntax
	const Type& operator()() const {
		return data;
	}
	Object& operator()(const Type& value) {
		data = value;
		return *object;
	}
	Object& operator()(Type&& value) {
		data = std::forward<Type>(value);
		return *object;
	}

	// access with get()/set() syntax
	const Type& get() const {
		return data;
	}
	const Type& set(const Type& value) {
		data = value;
		return data;
	}
	const Type& set(Type&& value) {
		data = std::forward<Type>(value);
		return data;
	}

	// access with '=' sign
	operator const Type&() const {
		return data;
	}
	const Type& operator=(const Type& value) {
		data = value;
		return data;
	}
	const Type& operator=(Type&& value) {
		data = std::move(value);
		return data;
	}

	// additional operations
	const Type& operator+=(const Type& value) {
		data += value;
		return data;
	}
	const Type& operator+=(Type&& value) {
		data += std::move(value);
		return data;
	}
	const Type& operator-=(const Type& value) {
		data -= value;
		return data;
	}
	const Type& operator-=(Type&& value) {
		data -= std::move(value);
		return data;
	}
	const Type& operator*=(const Type& value) {
		data *= value;
		return data;
	}
	const Type& operator*=(Type&& value) {
		data *= std::move(value);
		return data;
	}
	const Type& operator/=(const Type& value) {
		data /= value;
		return data;
	}
	const Type& operator/=(Type&& value) {
		data /= std::move(value);
		return data;
	}

	// comparators
	bool operator==(const Type& value) const {
		return get() == value;
	}
	bool operator!=(const Type& value) const {
		return not operator==(value);
	}

	// might be useful for template deductions
	typedef Type type;

};

template <class Type, class Object>
std::ostream& operator<<(std::ostream& output, const Property<Type, Object> simple_property) {
	return (output << simple_property.get());
}

// a read-only pointer property
// usage:
// ReadonlyPointerProperty<std::string> name;
// (has to be initialized with an address, for example in constructor using "name(pointer_to_string)")
template <typename Type>
class ReadonlyPointerProperty {

	Type* pointer;

public:

	ReadonlyPointerProperty(Type* pointer = nullptr) : pointer(pointer) {}

	ReadonlyPointerProperty(const ReadonlyPointerProperty&) = delete;
	ReadonlyPointerProperty(ReadonlyPointerProperty&&) = delete;

	ReadonlyPointerProperty& operator=(const ReadonlyPointerProperty&) = delete;
	ReadonlyPointerProperty& operator=(ReadonlyPointerProperty&&) = delete;

	// initializer to specify the owner
	void operator()(Type* pointer) {
		runtime_assert(this->pointer == nullptr, "property has already been initialized");
		runtime_assert(pointer != nullptr, "pointer must not be null");
		this->pointer = pointer;
	}

	//// function call syntax
	Type& operator()() const {
		return *pointer;
	}

	// get/set syntax
	Type& get() const {
		return *pointer;
	}

	// access with '=' sign
	operator Type&() const {
		return *pointer;
	}

	// access using pointer syntax
	Type* operator->() const {
		return pointer;
	}

	// comparators
	bool operator==(const Type& value) const {
		return get() == value;
	}
	bool operator!=(const Type& value) const {
		return not operator==(value);
	}
	bool operator==(const Type*const value) const {
		return pointer == value;
	}
	bool operator!=(const Type*const value) const {
		return not operator==(value);
	}

	// might be useful for template deductions
	typedef Type type;

};

template <class Type>
std::ostream& operator<<(std::ostream& output, const ReadonlyPointerProperty<Type> readonly_property) {
	return (output << readonly_property.get());
}

// a read-only property calling a user-defined getter, returning by value
// usage:
// ReadonlyValueProperty<std::string, MyClass, &MyClass::get_name> name;
// (has to be initialized with an owner, for example in constructor using "name(this)")
template <class Type, class Object, Type(Object::*real_getter)() const>
class ReadonlyValueProperty {

	Object* object;

public:

	ReadonlyValueProperty(Object* owner_object = nullptr) : object(owner_object) {}

	ReadonlyValueProperty(const ReadonlyValueProperty&) = delete;
	ReadonlyValueProperty(ReadonlyValueProperty&&) = delete;

	ReadonlyValueProperty& operator=(const ReadonlyValueProperty&) = delete;
	ReadonlyValueProperty& operator=(ReadonlyValueProperty&&) = delete;

	// initializer to specify the owner
	void operator()(Object* owner_object) {
		runtime_assert(object == nullptr, "property has already been initialized");
		runtime_assert(owner_object != nullptr, "owner_object must not be null");
		this->object = owner_object;
	}

	// function call syntax
	Type operator()() const {
		return (object->*real_getter)();
	}

	// get/set syntax
	Type get() const {
		return (object->*real_getter)();
	}

	// access with '=' sign
	operator Type() const {
		return (object->*real_getter)();
	}

	// comparators
	bool operator==(Type value) const {
		return get() == value;
	}
	bool operator!=(Type value) const {
		return not operator==(value);
	}

	// might be useful for template deductions
	typedef Type type;

};

template <class Type, class Object, Type(Object::*real_getter)() const>
std::ostream& operator<<(std::ostream& output, const ReadonlyValueProperty<Type, Object, real_getter> readonly_property) {
	return (output << readonly_property.get());
}

// a write-only property calling a user-defined setter, passing by value
// usage:
// WriteonlyValueProperty<std::string, MyClass, &MyClass::set_name> name;
// (has to be initialized with an owner, for example in constructor using "name(this)")
template <class Type, class Object, void(Object::*real_setter)(Type)>
class WriteonlyValueProperty {

	Object* object;

public:

	WriteonlyValueProperty(Object* owner_object = nullptr) : object(owner_object) {}

	WriteonlyValueProperty(const WriteonlyValueProperty&) = delete;
	WriteonlyValueProperty(WriteonlyValueProperty&&) = delete;

	WriteonlyValueProperty& operator=(const WriteonlyValueProperty&) = delete;
	WriteonlyValueProperty& operator=(WriteonlyValueProperty&&) = delete;

	// initializer to specify the owner
	void operator()(Object* owner_object) {
		runtime_assert(object == nullptr, "property has already been initialized");
		runtime_assert(owner_object != nullptr, "owner_object must not be null");
		this->object = owner_object;
	}

	// function call syntax
	void operator()(Type value) {
		return (object->*real_setter)(value);
	}

	// get/set syntax
	void set(Type value) {
		return (object->*real_setter)(value);
	}

	// access with '=' sign
	void operator=(Type value) {
		return (object->*real_setter)(value);
	}

	// might be useful for template deductions
	typedef Type type;

};

template <class Type, class Object, Type(Object::*real_setter)(Type)>
std::ostream& operator<<(std::ostream& output, const WriteonlyValueProperty<Type, Object, real_setter> writeonly_property) {
	return (output << writeonly_property.get());
}

// a read-write property that invokes user-defined functions, returning and passing by value
// usage:
// UnrestrictedValueProperty<std::string, MyClass, &MyClass::get_name, &MyClass::set_name> name;
// (has to be initialized with an owner, for example in constructor using "name(this)")
template <class Type, class Object, Type(Object::*real_getter)() const, Type(Object::*real_setter)(Type)>
class UnrestrictedValueProperty {

	Object* object;

public:

	UnrestrictedValueProperty(Object* owner_object = nullptr) : object(owner_object) {}

	UnrestrictedValueProperty(const UnrestrictedValueProperty&) = delete;
	UnrestrictedValueProperty(UnrestrictedValueProperty&&) = delete;

	UnrestrictedValueProperty& operator=(const UnrestrictedValueProperty&) = delete;
	UnrestrictedValueProperty& operator=(UnrestrictedValueProperty&&) = delete;

	// initializer to specify the owner
	void operator()(Object* owner_object) {
		runtime_assert(object == nullptr, "property has already been initialized");
		runtime_assert(owner_object != nullptr, "owner_object must not be null");
		this->object = owner_object;
	}

	// function call syntax
	Type operator()() const {
		return (object->*real_getter)();
	}
	Type operator()(Type value) {
		return (object->*real_setter)(value);
	}

	// get/set syntax
	Type get() const {
		return (object->*real_getter)();
	}
	Type set(Type value) {
		return (object->*real_setter)(value);
	}

	// access with '=' sign
	operator Type() const {
		return (object->*real_getter)();
	}
	Type operator=(Type value) {
		return (object->*real_setter)(value);
	}

	// comparators
	bool operator==(Type value) const {
		return get() == value;
	}
	bool operator!=(Type value) const {
		return not operator==(value);
	}

	// might be useful for template deductions
	typedef Type type;

};

template <class Type, class Object, Type(Object::*real_getter)() const, Type(Object::*real_setter)(Type)>
std::ostream& operator<<(std::ostream& output, const UnrestrictedValueProperty<Type, Object, real_getter, real_setter> unrestricted_property) {
	return (output << unrestricted_property.get());
}

// a read-only property calling a user-defined getter, returning by reference and passing by perfect-forwarding
// usage:
// ReadonlyReferenceProperty<std::string, MyClass, &MyClass::get_name> name;
// (has to be initialized with an owner, for example in constructor using "name(this)")
template <typename Type, typename Object, const Type&(Object::*real_getter)() const>
class ReadonlyReferenceProperty {

	Object* object;

public:

	ReadonlyReferenceProperty(Object* owner_object = nullptr) : object(owner_object) {}

	ReadonlyReferenceProperty(const ReadonlyReferenceProperty&) = delete;
	ReadonlyReferenceProperty(ReadonlyReferenceProperty&&) = delete;

	ReadonlyReferenceProperty& operator=(const ReadonlyReferenceProperty&) = delete;
	ReadonlyReferenceProperty& operator=(ReadonlyReferenceProperty&&) = delete;

	// initializer to specify the owner
	void operator()(Object* owner_object) {
		runtime_assert(object == nullptr, "property has already been initialized");
		runtime_assert(owner_object != nullptr, "owner_object must not be null");
		this->object = owner_object;
	}

	// function call syntax
	const Type& operator()() const {
		return (object->*real_getter)();
	}

	// get/set syntax
	const Type& get() const {
		return (object->*real_getter)();
	}

	// access with '=' sign
	operator const Type&() const {
		return (object->*real_getter)();
	}

	// comparators
	bool operator==(const Type& value) const {
		return get() == value;
	}
	bool operator!=(const Type& value) const {
		return not operator==(value);
	}

	// might be useful for template deductions
	typedef Type type;

};

template <class Type, class Object, const Type&(Object::*real_getter)() const>
std::ostream& operator<<(std::ostream& output, const ReadonlyReferenceProperty<Type, Object, real_getter> readonly_property) {
	return (output << readonly_property.get());
}

// a write-only property calling a user-defined setter, passing by perfect-forwarding
// usage:
// WriteonlyReferenceProperty<std::string, MyClass, &MyClass::set_name> name;
// (has to be initialized with an owner, for example in constructor using "name(this)")
template <class Type, class Object, void(Object::*real_setter)(Assignment<Type>)>
class WriteonlyReferenceProperty {

	Object* object;

public:

	WriteonlyReferenceProperty(Object* owner_object = nullptr) : object(owner_object) {}

	WriteonlyReferenceProperty(const WriteonlyReferenceProperty&) = delete;
	WriteonlyReferenceProperty(WriteonlyReferenceProperty&&) = delete;

	WriteonlyReferenceProperty& operator=(const WriteonlyReferenceProperty&) = delete;
	WriteonlyReferenceProperty& operator=(WriteonlyReferenceProperty&&) = delete;

	// initializer to specify the owner
	void operator()(Object* owner_object) {
		runtime_assert(object == nullptr, "property has already been initialized");
		runtime_assert(owner_object != nullptr, "owner_object must not be null");
		this->object = owner_object;
	}

	// function call syntax
	void operator()(const Type& value) {
		(object->*real_setter)(value);
	}
	void operator()(Type&& value) {
		(object->*real_setter)(std::move(value));
	}

	// get/set syntax
	void set(const Type& value) {
		(object->*real_setter)(value);
	}
	void set(Type&& value) {
		(object->*real_setter)(std::move(value));
	}

	// access with '=' sign
	void operator=(const Type& value) {
		(object->*real_setter)(value);
	}
	void operator=(Type&& value) {
		(object->*real_setter)(std::move(value));
	}

	// might be useful for template deductions
	typedef Type type;

};

template <class Type, class Object, void(Object::*real_setter)(Assignment<Type>)>
std::ostream& operator<<(std::ostream& output, const WriteonlyReferenceProperty<Type, Object, real_setter> writeonly_property) {
	return (output << writeonly_property.get());
}

// a read-write property that invokes user-defined functions, returning by reference and passing by perfect-forwarding
// usage:
// UnrestrictedReferenceProperty<std::string, MyClass, &MyClass::get_name, &MyClass::set_name> name;
// (has to be initialized with an owner, for example in constructor using "name(this)")
template <class Type, class Object, const Type&(Object::*real_getter)() const, const Type&(Object::*real_setter)(Assignment<Type>)>
class UnrestrictedReferenceProperty {

	Object* object;

public:

	UnrestrictedReferenceProperty(Object* owner_object = nullptr) : object(owner_object) {}

	UnrestrictedReferenceProperty(const UnrestrictedReferenceProperty&) = delete;
	UnrestrictedReferenceProperty(UnrestrictedReferenceProperty&&) = delete;

	UnrestrictedReferenceProperty& operator=(const UnrestrictedReferenceProperty&) = delete;
	UnrestrictedReferenceProperty& operator=(UnrestrictedReferenceProperty&&) = delete;

	// initializer to specify the owner
	void operator()(Object* owner_object) {
		runtime_assert(object == nullptr, "property has already been initialized");
		runtime_assert(owner_object != nullptr, "owner_object must not be null");
		this->object = owner_object;
	}

	// function call syntax
	const Type& operator()() const {
		return (object->*real_getter)();
	}
	const Type& operator()(const Type& value) {
		return (object->*real_setter)(value);
	}
	const Type& operator()(Type&& value) {
		return (object->*real_setter)(std::move(value));
	}

	// get/set syntax
	const Type& get() const {
		return (object->*real_getter)();
	}
	const Type& set(const Type& value) {
		return (object->*real_setter)(value);
	}
	const Type& set(Type&& value) {
		return (object->*real_setter)(std::move(value));
	}

	// access with '=' sign
	operator const Type&() const {
		return (object->*real_getter)();
	}
	const Type& operator=(const Type& value) {
		return (object->*real_setter)(value);
	}
	const Type& operator=(Type&& value) {
		return (object->*real_setter)(std::move(value));
	}

	// comparators
	bool operator==(const Type& value) const {
		return get() == value;
	}
	bool operator!=(const Type& value) const {
		return not operator==(value);
	}

	// might be useful for template deductions
	typedef Type type;

};

template <class Type, class Object, const Type&(Object::*real_getter)() const, const Type&(Object::*real_setter)(Assignment<Type>)>
std::ostream& operator<<(std::ostream& output, const UnrestrictedReferenceProperty<Type, Object, real_getter, real_setter> unrestricted_property) {
	return (output << unrestricted_property.get());
}
