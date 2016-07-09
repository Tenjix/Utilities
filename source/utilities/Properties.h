#pragma once

#include <functional>
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>

#include <utilities/Assertions.h>
#include <utilities/Logging.h>

// ==================================================================================================
// Assignment using type-erasure
// ==================================================================================================

template<class Type>
class Assignment {

	std::function<void(Type&)> assign_to;

public:

	Assignment(const Type& value) : assign_to([&value](Type& target) { target = value; }) {}
	Assignment(Type&& value) : assign_to([&value](Type& target) { target = std::move(value); }) {}
	Assignment(Assignment&&) = default;

	const Type& to(Type& target) const { assign_to(target); return target; }

};

// ==================================================================================================
// Property base classes
// ==================================================================================================

namespace {

	// readable property tag for common functions
	class Readable {};

	// writable property tag for common functions
	class Writable {};

	// property owner reference
	template <class Owner>
	class PropertyOwner {

		friend Owner;

		Owner* pointer = nullptr;

		void operator=(Owner* owner) {
			pointer = owner;
		}

	public:

		operator const Owner*() const {
			runtime_assert(pointer != nullptr, "property owner hasn't been specified");
			return pointer;
		}
		operator Owner*() {
			runtime_assert(pointer != nullptr, "property owner hasn't been specified");
			return pointer;
		}

		operator const Owner&() const {
			runtime_assert(pointer != nullptr, "property owner hasn't been specified");
			return *pointer;
		}
		operator Owner&() {
			runtime_assert(pointer != nullptr, "property owner hasn't been specified");
			return *pointer;
		}

	};

	// property base containing owner reference
	template <class Type, class Owner>
	class BaseProperty {

	public:

		PropertyOwner<Owner> owner;

		typedef Owner owner_type;
		typedef Type value_type;

	};

	// property base containing owner reference and data storage
	template <class Type, class Owner>
	class DataProperty : public BaseProperty<Type, Owner> {

	protected:

		Type value;

	public:

		DataProperty() : value() {}

		DataProperty(const Type& value) : value(value) {}
		DataProperty(Type&& value) : value(std::move(value)) {}

		DataProperty(const DataProperty& other) : value(other.value) {}
		DataProperty(DataProperty&& other) : value(std::move(other.value)) {}

		DataProperty& operator=(const DataProperty& other) { value = other.value; return *this; }
		DataProperty& operator=(DataProperty&& other) { value = std::move(other.value); return *this; }

	};

}

// ==================================================================================================
// Properties with value storage
// ==================================================================================================

// a read-only property with value storage and various getter functions
// usage:
// in public section of some class (e.g. "Entity"): "ReadonlyProperty<std::string, Entity> name;"
// (has to be initialized with an owner, usually in the constructor using "name.owner = this;")
template <class Type, class Owner>
class ReadonlyProperty : Readable, public virtual DataProperty<Type, Owner> {

public:

	ReadonlyProperty() : DataProperty<Type, Owner>() {}

	explicit ReadonlyProperty(const Type& value) : DataProperty<Type, Owner>(value) {}
	explicit ReadonlyProperty(Type&& value) : DataProperty<Type, Owner>(std::move(value)) {}

	ReadonlyProperty(const ReadonlyProperty& other) : ReadonlyProperty(other.value) {}
	ReadonlyProperty(ReadonlyProperty&& other) : ReadonlyProperty(std::move(other.value)) {}

	ReadonlyProperty& operator=(const ReadonlyProperty& other) { value = other.value; return *this; }
	ReadonlyProperty& operator=(ReadonlyProperty&& other) { value = std::move(other.value); return *this; }

	// access with function call syntax
	const Type& operator()() const {
		return value;
	}
	Type& operator()() {
		return value;
	}

	// access with get()/set() syntax
	const Type& get() const {
		return value;
	}
	Type& get() {
		return value;
	}

	// conversion
	operator const Type&() const {
		return value;
	}
	operator Type&() {
		return value;
	}

	// comparation
	bool operator==(const Type& value) const {
		return get() == value;
	}
	bool operator!=(const Type& value) const {
		return not operator==(value);
	}

	// member access using "->" until "." can be overloaded
	const Type* operator->() const {
		return &value;
	}
	Type* operator->() {
		return &value;
	}

};

// a write-only property with value storage and various setter functions
// usage:
// in public section of some class (e.g. "Entity"): "WriteonlyProperty<std::string, Entity> name;"
// (has to be initialized with an owner, usually in the constructor using "name.owner = this;")
template <class Type, class Owner>
class WriteonlyProperty : Writable, public virtual DataProperty<Type, Owner> {

public:

	WriteonlyProperty() : DataProperty<Type, Owner>() {}

	explicit WriteonlyProperty(const Type& value) : DataProperty<Type, Owner>(value) {}
	explicit WriteonlyProperty(Type&& value) : DataProperty<Type, Owner>(std::move(value)) {}

	WriteonlyProperty(const WriteonlyProperty& other) : WriteonlyProperty(other.value) {}
	WriteonlyProperty(WriteonlyProperty&& other) : WriteonlyProperty(std::move(other.value)) {}

	WriteonlyProperty& operator=(const WriteonlyProperty& other) { value = other.value; return *this; }
	WriteonlyProperty& operator=(WriteonlyProperty&& other) { value = std::move(other.value); return *this; }

	// access with function call syntax
	Owner& operator()(const Type& value) {
		this->value = value;
		return owner;
	}
	Owner& operator()(Type&& value) {
		this->value = std::move(value);
		return owner;
	}

	// access with get()/set() syntax
	void set(const Type& value) {
		this->value = value;
	}
	void set(Type&& value) {
		this->value = std::move(value);
	}

	// assignment
	void operator=(const Type& value) {
		this->value = value;
	}
	void operator=(Type&& value) {
		this->value = std::move(value);
	}

	// manipulation
	void operator+=(const Type& value) {
		this->value += value;
	}
	void operator-=(const Type& value) {
		this->value -= value;
	}
	void operator*=(const Type& value) {
		this->value *= value;
	}
	void operator/=(const Type& value) {
		this->value /= value;
	}

};

// a read-write property with value storage and various accessor functions
// usage:
// in public section of some class (e.g. "Entity"): "Property<std::string, Entity> name;"
// (has to be initialized with an owner, usually in the constructor using "name.owner = this;")
template <class Type, class Owner>
class Property : public ReadonlyProperty<Type, Owner>, public WriteonlyProperty<Type, Owner> {

public:

	Property() : DataProperty<Type, Owner>() {}

	explicit Property(const Type& value) : DataProperty<Type, Owner>(value) {}
	explicit Property(Type&& value) : DataProperty<Type, Owner>(std::move(value)) {}

	Property(const Property& other) : Property(other.value) {}
	Property(Property&& other) : Property(std::move(other.value)) {}

	Property& operator=(const Property& other) { value = other.value; return *this; }
	Property& operator=(Property&& other) { value = std::move(other.value); return *this; }

	using ReadonlyProperty<Type, Owner>::operator();
	using WriteonlyProperty<Type, Owner>::operator();

	// assignment
	Type& operator=(const Type& value) {
		return this->value = value;
	}
	Type& operator=(Type&& value) {
		return this->value = std::move(value);
	}

	// manipulation
	Type& operator+=(const Type& value) {
		return this->value += value;
	}
	Type& operator-=(const Type& value) {
		return this->value -= value;
	}
	Type& operator*=(const Type& value) {
		return this->value *= value;
	}
	Type& operator/=(const Type& value) {
		return this->value /= value;
	}

};

// ==================================================================================================
// Properties with shared pointer
// ==================================================================================================

// a read-write property with value storage of a shared pointer and various accessor functions
// usage:
// in public section of some class (e.g. "Entity"): "SharedProperty<std::string, Entity> name;"
// (has to be initialized with an owner, usually in the constructor using "name.owner = this;")
template <class Type, class Owner>
class SharedProperty : public Property<shared<Type>, Owner> {

public:

	SharedProperty() : DataProperty<shared<Type>, Owner>() {}

	explicit SharedProperty(const shared<Type>& value) : DataProperty<shared<Type>, Owner>(value) {}
	explicit SharedProperty(shared<Type>&& value) : DataProperty<shared<Type>, Owner>(std::move(value)) {}

	SharedProperty(const SharedProperty& other) : SharedProperty(other.value) {}
	SharedProperty(SharedProperty&& other) : SharedProperty(std::move(other.value)) {}

	SharedProperty& operator=(const SharedProperty& other) { value = other.value; return *this; }
	SharedProperty& operator=(SharedProperty&& other) { value = std::move(other.value); return *this; }

	using Property<shared<Type>, Owner>::operator=;

	// returns address of the shared value
	const Type* address() const {
		return value.get();
	}
	Type* address() {
		return value.get();
	}

	// access using pointer syntax
	const Type* operator->() const {
		return value.get();
	}
	Type* operator->() {
		return value.get();
	}
	const Type& operator*() const {
		return *value;
	}
	Type& operator*() {
		return *value;
	}

	// conversion
	operator bool() const {
		return value != nullptr;
	}

	typedef Type shared_type;

};

// ==================================================================================================
// Properties with raw pointer
// ==================================================================================================

// a read-only property with value storage of a raw pointer and various getter functions
// usage:
// in public section of some class (e.g. "Entity"): "ReadonlyPointerProperty<std::string, Entity> name;"
// (has to be initialized with an pointer, usually in the constructor using "name.pointer = &some_persistent_string;")
template <class Type, class Owner>
class ReadonlyPointerProperty : Readable {

	friend Owner;

	Type* pointer;

public:

	explicit ReadonlyPointerProperty(Type* pointer = nullptr) : pointer(pointer) {}

	// access with get()/set() syntax
	const Type* get() const {
		return pointer;
	}
	Type* get() {
		return pointer;
	}

	// conversion
	operator const Type*() const {
		return pointer;
	}
	operator Type*() {
		return pointer;
	}
	explicit operator bool() const {
		return value != nullptr;
	}

	// access using pointer syntax
	const Type* operator->() const {
		return pointer;
	}
	Type* operator->() {
		return pointer;
	}
	const Type& operator*() const {
		return *pointer;
	}
	Type& operator*() {
		return *pointer;
	}

	// comparation
	bool operator==(const Type* other) const {
		return pointer == other;
	}
	bool operator!=(const Type* other) const {
		return not operator==(other);
	}

	typedef Type type;

};

// ==================================================================================================
// Properties passing by value
// ==================================================================================================

// a read-only property calling a user-defined getter, returning by value
// usage:
// in public section of some class (e.g. "Entity"): "ReadonlyByValueProperty<unsigned, Entity, &Entity::get_id> id;"
// (has to be initialized with an owner, usually in the constructor using "id.owner = this;")
template <class Type, class Owner, Type(Owner::*getter)() const>
class ReadonlyByValueProperty : Readable, public virtual BaseProperty<Type, Owner> {

public:

	ReadonlyByValueProperty() : BaseProperty<Type, Owner>() {}

	ReadonlyByValueProperty(const ReadonlyByValueProperty& other) {}
	ReadonlyByValueProperty(ReadonlyByValueProperty&& other) {}

	ReadonlyByValueProperty& operator=(const ReadonlyByValueProperty& other) { return *this; }
	ReadonlyByValueProperty& operator=(ReadonlyByValueProperty&& other) { return *this; }

	// access with function call syntax
	const Type operator()() const {
		return (owner->*getter)();
	}
	Type operator()() {
		return (owner->*getter)();
	}

	// access with get()/set() syntax
	const Type get() const {
		return (owner->*getter)();
	}
	Type get() {
		return (owner->*getter)();
	}

	// conversion
	operator const Type() const {
		return (owner->*getter)();
	}
	operator Type() {
		return (owner->*getter)();
	}

	// comparation
	bool operator==(const Type& value) const {
		return (owner->*getter)() == value;
	}
	bool operator!=(const Type& value) const {
		return not operator==(value);
	}

};

// a write-only property calling a user-defined setter, passing by value
// usage:
// in public section of some class (e.g. "Entity"): "WriteonlyByValueProperty<unsigned, Entity, &Entity::set_id> id;"
// (has to be initialized with an owner, usually in the constructor using "id.owner = this;")
template <class Type, class Owner, void(Owner::*setter)(Type)>
class WriteonlyByValueProperty : Writable, public virtual BaseProperty<Type, Owner> {

public:

	WriteonlyByValueProperty() : BaseProperty<Type, Owner>() {}

	WriteonlyByValueProperty(const WriteonlyByValueProperty& other) {}
	WriteonlyByValueProperty(WriteonlyByValueProperty&& other) {}

	WriteonlyByValueProperty& operator=(const WriteonlyByValueProperty& other) { return *this; }
	WriteonlyByValueProperty& operator=(WriteonlyByValueProperty&& other) { return *this; }

	// access with function call syntax
	Owner& operator()(Type value) {
		(owner->*setter)(value);
		return owner;
	}

	// access with get()/set() syntax
	void set(Type value) {
		(owner->*setter)(value);
	}

	// assignment
	void operator=(Type value) {
		(owner->*setter)(value);
	}

};

// a read-write property that invokes user-defined accessors, returning and passing by value
// usage:
// in public section of some class (e.g. "Entity"): "ByValueProperty<unsigned, Entity, &Entity::get_id, &Entity::set_id> id;"
// (has to be initialized with an owner, usually in the constructor using "id.owner = this;")
template <class Type, class Owner, Type(Owner::*getter)() const, void(Owner::*setter)(Type)>
class ByValueProperty : public ReadonlyByValueProperty<Type, Owner, getter>, public WriteonlyByValueProperty<Type, Owner, setter> {

public:

	ByValueProperty() : BaseProperty<Type, Owner>() {}

	ByValueProperty(const ByValueProperty& other) {}
	ByValueProperty(ByValueProperty&& other) {}

	ByValueProperty& operator=(const ByValueProperty& other) { return *this; }
	ByValueProperty& operator=(ByValueProperty&& other) { return *this; }

	using ReadonlyByValueProperty<Type, Owner, getter>::operator();
	using WriteonlyByValueProperty<Type, Owner, setter>::operator();

	// assignment
	Type operator=(Type value) {
		(owner->*setter)(value);
		return get();
	}

	// manipulation
	Type operator+=(const Type& value) {
		set(get() + value);
		return get();
	}
	Type operator-=(const Type& value) {
		set(get() - value);
		return get();
	}
	Type operator*=(const Type& value) {
		set(get() * value);
		return get();
	}
	Type operator/=(const Type& value) {
		set(get() / value);
		return get();
	}

};

// ==================================================================================================
// Properties passing by reference
// ==================================================================================================

// a read-only property calling a user-defined getter, returning by reference
// usage:
// in public section of some class (e.g. "Entity"): "ReadonlyByReferenceProperty<std::string, Entity, &Entity::get_name> name;"
// (has to be initialized with an owner, usually in the constructor using "name.owner = this;")
template <class Type, class Owner, const Type&(Owner::*getter)() const>
class ReadonlyByReferenceProperty : Readable, public virtual BaseProperty<Type, Owner> {

public:

	ReadonlyByReferenceProperty() : BaseProperty<Type, Owner>() {}

	ReadonlyByReferenceProperty(const ReadonlyByReferenceProperty& other) {}
	ReadonlyByReferenceProperty(ReadonlyByReferenceProperty&& other) {}

	ReadonlyByReferenceProperty& operator=(const ReadonlyByReferenceProperty& other) { return *this; }
	ReadonlyByReferenceProperty& operator=(ReadonlyByReferenceProperty&& other) { return *this; }

	// access with function call syntax
	const Type& operator()() const {
		return get();
	}
	Type& operator()() {
		return get();
	}

	// access with get()/set() syntax
	const Type& get() const {
		return (owner->*getter)();
	}
	Type& get() {
		return const_cast<Type&>((owner->*getter)());
	}

	// conversion
	operator const Type&() const {
		return get();
	}
	operator Type&() {
		return get();
	}

	// comparation
	bool operator==(const Type& value) const {
		return get() == value;
	}
	bool operator!=(const Type& value) const {
		return not operator==(value);
	}

};

// a write-only property calling a user-defined setter, passing by reference
// usage:
// in public section of some class (e.g. "Entity"): "WriteonlyByReferenceProperty<std::string, Entity, &Entity::set_name> name;"
// (has to be initialized with an owner, usually in the constructor using "name.owner = this;")
template <class Type, class Owner, void(Owner::*setter)(Assignment<Type>)>
class WriteonlyByReferenceProperty : Writable, public virtual BaseProperty<Type, Owner> {

public:

	WriteonlyByReferenceProperty() : BaseProperty<Type, Owner>() {}

	WriteonlyByReferenceProperty(const WriteonlyByReferenceProperty& other) {}
	WriteonlyByReferenceProperty(WriteonlyByReferenceProperty&& other) {}

	WriteonlyByReferenceProperty& operator=(const WriteonlyByReferenceProperty& other) { return *this; }
	WriteonlyByReferenceProperty& operator=(WriteonlyByReferenceProperty&& other) { return *this; }

	// access with function call syntax
	Owner& operator()(const Type& value) {
		set(value);
		return owner;
	}
	Owner& operator()(Type&& value) {
		set(std::move(value));
		return owner;
	}

	// access with get()/set() syntax
	void set(const Type& value) {
		(owner->*setter)(value);
	}
	void set(Type&& value) {
		(owner->*setter)(std::move(value));
	}

	// assignment
	void operator=(const Type& value) {
		set(value);
	}
	void operator=(Type&& value) {
		set(std::move(value));
	}

};

// a read-write property that invokes user-defined functions, returning by reference
// usage:
// in public section of some class (e.g. "Entity"): "ByReferenceProperty<std::string, Entity, &Entity::get_name, &Entity::set_name> name;"
// (has to be initialized with an owner, usually in the constructor using "name.owner = this;")
template <class Type, class Owner, const Type&(Owner::*getter)() const, void(Owner::*setter)(Assignment<Type>)>
class ByReferenceProperty : public ReadonlyByReferenceProperty<Type, Owner, getter>, public WriteonlyByReferenceProperty<Type, Owner, setter> {

public:

	ByReferenceProperty() : BaseProperty<Type, Owner>() {}

	ByReferenceProperty(const ByReferenceProperty& other) {}
	ByReferenceProperty(ByReferenceProperty&& other) {}

	ByReferenceProperty& operator=(const ByReferenceProperty& other) { return *this; }
	ByReferenceProperty& operator=(ByReferenceProperty&& other) { return *this; }

	using ReadonlyByReferenceProperty<Type, Owner, getter>::operator();
	using WriteonlyByReferenceProperty<Type, Owner, setter>::operator();

	// assignment
	Type& operator=(const Type& value) {
		(owner->*setter)(value);
		return get();
	}
	Type& operator=(Type&& value) {
		(owner->*setter)(std::move(value));
		return get();
	}

	// manipulation
	Type& operator+=(const Type& value) {
		set(get() + value);
		return get();
	}
	Type& operator+=(Type&& value) {
		set(get() + std::move(value));
		return get();
	}
	Type& operator-=(const Type& value) {
		set(get() - value);
		return get();
	}
	Type& operator-=(Type&& value) {
		set(get() - std::move(value));
		return get();
	}
	Type& operator*=(const Type& value) {
		set(get() * value);
		return get();
	}
	Type& operator*=(Type&& value) {
		set(get() * std::move(value));
		return get();
	}
	Type& operator/=(const Type& value) {
		set(get() / value);
		return get();
	}
	Type& operator/=(Type&& value) {
		set(get() / std::move(value));
		return get();
	}

};

// ==================================================================================================
// Common property operators
// ==================================================================================================

// output stream operator<<
template <class PropertyType, typename std::enable_if<std::is_base_of<Readable, PropertyType>::value>::type* = nullptr>
std::ostream& operator<<(std::ostream& output, const PropertyType& property) {
	return output << property.get();
}

// string concatenation (string + property)
template <class PropertyType, typename std::enable_if<std::is_base_of<Readable, PropertyType>::value>::type* = nullptr>
std::string operator+(const std::string& text, const PropertyType& property) {
	return text + to_string(property.get());
}

// string concatenation (property + string)
template <class PropertyType, typename std::enable_if<std::is_base_of<Readable, PropertyType>::value>::type* = nullptr>
std::string operator+(const PropertyType& property, const std::string& text) {
	return to_string(property.get()) + text;
}

// string concatenation (characters + property)
template <class PropertyType, typename std::enable_if<std::is_base_of<Readable, PropertyType>::value>::type* = nullptr>
std::string operator+(const char* text, const PropertyType& property) {
	return string(text) + to_string(property.get());
}

// string concatenation (property + characters)
template <class PropertyType, typename std::enable_if<std::is_base_of<Readable, PropertyType>::value>::type* = nullptr>
std::string operator+(const PropertyType& property, const char* text) {
	return to_string(property.get()) + string(text);
}
