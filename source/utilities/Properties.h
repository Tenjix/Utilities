#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <utility>

#include <utilities/Assertions.h>

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
// (has to be initialized with an owner, for example in constructor using "name._property_owner(pointer_to_string)")
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
	void _property_owner(Object* owner_object) {
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
		data = std::move(value);
		return *object;
	}

	// access with get()/set() syntax
	const Type& get() const {
		return data;
	}
	const Type& set(const Type& value) {
		return data = value;
	}
	const Type& set(Type&& value) {
		return data = std::move(value);
	}

	// access with '=' sign
	operator const Type&() const {
		return data;
	}
	const Type& operator=(const Type& value) {
		return data = value;
	}
	const Type& operator=(Type&& value) {
		return data = std::move(value);
	}

	// additional operations
	const Type& operator+=(const Type& value) {
		return data += value;
	}
	const Type& operator-=(const Type& value) {
		return data -= value;
	}
	const Type& operator*=(const Type& value) {
		return data *= value;
	}
	const Type& operator/=(const Type& value) {
		return data /= value;
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

// a read-write shared pointer property
// usage:
// SharedProperty<std::string, OwnerClass> name;
// (has to be initialized with an owner, for example in constructor using "name._property_owner(this)")
template <class Type, class Object>
class SharedPointerProperty {

	shared<Type> pointer;
	Object* object;

public:

	SharedPointerProperty(Object* owner_object = nullptr) : object(owner_object) {}
	SharedPointerProperty(Type* pointer) : pointer(pointer) {}

	SharedPointerProperty(const SharedPointerProperty&) = delete;
	SharedPointerProperty(SharedPointerProperty&&) = delete;

	SharedPointerProperty& operator=(const SharedPointerProperty&) = delete;
	SharedPointerProperty& operator=(SharedPointerProperty&&) = delete;

	// initializer to specify the owner
	void _property_owner(Object* owner_object) {
		runtime_assert(object == nullptr, "property has already been initialized");
		runtime_assert(owner_object != nullptr, "owner_object must not be null");
		this->object = owner_object;
	}

	// function call syntax
	const shared<Type>& operator()() const {
		return pointer;
	}
	Object& operator()(const shared<Type>& value) {
		pointer = value;
		return *object;
	}
	Object& operator()(shared<Type>&& value) {
		pointer = std::move(value);
		return *object;
	}

	// get/set syntax
	const shared<Type>& get() const {
		return pointer;
	}
	const shared<Type>& set(const shared<Type>& value) {
		return pointer = value;
	}
	const shared<Type>& set(shared<Type>&& value) {
		return pointer = std::move(value);
	}

	// access with '=' sign
	operator const shared<Type>&() const {
		return pointer;
	}
	const shared<Type>& operator=(const shared<Type>& value) {
		return pointer = value;
	}
	const shared<Type>& operator=(shared<Type>&& value) {
		return pointer = std::move(value);
	}

	// access using pointer syntax
	Type* operator->() const {
		return pointer.get();
	}
	Type& operator*() const {
		return *pointer;
	}

	// comparators
	bool operator==(const shared<Type>& value) const {
		return get() == value;
	}
	bool operator!=(const shared<Type>& value) const {
		return not operator==(value);
	}

	// might be useful for template deductions
	typedef Type type;

};

template <class Type, class Object>
std::ostream& operator<<(std::ostream& output, const SharedPointerProperty<Type, Object> shared_pointer_property) {
	if (shared_pointer_property == nullptr) return (output << "nullptr");
	return (output << *shared_pointer_property);
}

// a read-only pointer property
// usage:
// ReadonlyPointerProperty<std::string> name;
// (has to be initialized with an pointer, for example in constructor using "name._property_initialize(pointer_to_string)")
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
	void _property_initialize(Type* pointer) {
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
	Type& operator*() const {
		return *pointer;
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
std::ostream& operator<<(std::ostream& output, const ReadonlyPointerProperty<Type> readonly_pointer_property) {
	if (readonly_pointer_property == nullptr) return (output << "nullptr");
	return (output << *readonly_pointer_property);
}

// a read-only property calling a user-defined getter, returning by value
// usage:
// ReadonlyValueProperty<std::string, OwnerClass, &OwnerClass::get_name> name;
// (has to be initialized with an owner, for example in constructor using "name._property_owner(this)")
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
	void _property_owner(Object* owner_object) {
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
// WriteonlyValueProperty<std::string, OwnerClass, &OwnerClass::set_name> name;
// (has to be initialized with an owner, for example in constructor using "name._property_owner(this)")
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
	void _property_owner(Object* owner_object) {
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
// UnrestrictedValueProperty<std::string, OwnerClass, &OwnerClass::get_name, &OwnerClass::set_name> name;
// (has to be initialized with an owner, for example in constructor using "name._property_owner(this)")
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
	void _property_owner(Object* owner_object) {
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
// ReadonlyReferenceProperty<std::string, OwnerClass, &OwnerClass::get_name> name;
// (has to be initialized with an owner, for example in constructor using "name._property_owner(this)")
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
	void _property_owner(Object* owner_object) {
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
// WriteonlyReferenceProperty<std::string, OwnerClass, &OwnerClass::set_name> name;
// (has to be initialized with an owner, for example in constructor using "name._property_owner(this)")
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
	void _property_owner(Object* owner_object) {
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
// UnrestrictedReferenceProperty<std::string, OwnerClass, &OwnerClass::get_name, &OwnerClass::set_name> name;
// (has to be initialized with an owner, for example in constructor using "name._property_owner(this)")
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
	void _property_owner(Object* owner_object) {
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
