#pragma once

#include <cmath>

// define important constants
constexpr double Pi = 3.14159265358979323846;
constexpr double Pi_Inverse = 0.318309886183790671538;
constexpr double Pi_Half = 1.57079632679489661923;
constexpr double Sqrt_2 = 1.41421356237309504880;
constexpr double Sqrt_2_Inverse = 0.707106781186547524401;
constexpr double Sqrt_3 = 1.73205080756887729353;
constexpr double Sqrt_3_Inverse = 0.577350269189625764509;

template <typename Type>
struct Range {

	Type minimum;
	Type maximum;

	Range() : minimum(), maximum() {}
	Range(const Type& minimum, const Type& maximum) : minimum(minimum), maximum(maximum) {}
	Range(Type&& minimum, Type&& maximum) : minimum(std::move(minimum)), maximum(std::move(maximum)) {}

	bool contains(const Type& value, bool including_limits = true) const {
		if (including_limits) return minimum <= value and value <= maximum;
		return minimum < value and value < maximum;
	}

	Type size() {
		return std::abs(maximum - minimum);
	}

};

// returns the maximum of two given values using the ">" operator
template <typename Type>
inline const Type& max(const Type& a, const Type& b) { return a > b ? a : b; }

// returns the minimum of two given values using the "<" operator
template <typename Type>
inline const Type& min(const Type& a, const Type& b) { return a < b ? a : b; }

// determines whether "value" is even using the "%" operator
template <typename Type>
inline bool even(const Type& value) { return value % 2 == 0; }

// determines whether "value" is odd using the "%" operator
template <typename Type>
inline bool odd(const Type& value) { return value % 2 != 0; }

// determines whether "value" is within the range "range_begin" to "range_end", including limits by default
template <typename Type>
inline bool within(const Type& value, const Type& range_begin, const Type& range_end, bool including_limits = true) {
	if (including_limits) return range_begin <= value and value <= range_end;
	return range_begin < value and value < range_end;
}

// calculates the signum function (negative => -1, zero => 0, positive => +1)
template <typename Type>
inline int signum(Type value) { return (Type(0) < value) - (value < Type(0)); }

// projects "value" into the wrapping range "range_begin" to "range_end"
// (e.g.: project(x, 0, 3) projects: ..,-4,0,4,.. -> 0; ..,-3,1,5,.. -> 1; ..,-2,2,6,.. -> 2; ..,-1,3,7,.. -> 3)
inline int project(int value, const int range_begin, const int range_end) {
	int range_size = (range_end + 1) - range_begin;
	if (value < range_begin) value += range_size * ((range_begin - value) / range_size + 1);
	return range_begin + (value - range_begin) % range_size;
}
