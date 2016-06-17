#pragma once

#include <iostream>

#include <utilities/Types.h>

// Type Filter
struct TypeFilter {

	Types required_types;
	Types excluded_types;

	TypeFilter() {}

	TypeFilter(Types requires, Types excludes = Types()) : required_types(requires), excluded_types(excludes) {}

	template <typename... TYPES>
	bool accepts() const {
		return accepts(Types<TYPES...>());
	}

	bool accepts(const Types& types) const {
		if (not excluded_types.empty() && types.contain(excluded_types)) return false;
		return types.contain(required_types);
	}

	friend std::ostream& operator<<(std::ostream& output, const TypeFilter& filter) {
		return (output << "requires<" << filter.required_types << ">, excludes<" << filter.excluded_types << ">");
	}

};
