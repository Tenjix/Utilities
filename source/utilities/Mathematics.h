#pragma once

#include <cmath>

// define important constants
namespace double_constants {
	constexpr double Tau = 6.28318530717958647693;
	constexpr double Tau_Inverse = 1.0 / Tau;
	constexpr double Tau_Half = Tau / 2.0;
	constexpr double Pi = Tau_Half;
	constexpr double Pi_Inverse = 1.0 / Pi;
	constexpr double Pi_Half = Pi / 2.0;
	constexpr double Sqrt_2 = 1.41421356237309504880;
	constexpr double Sqrt_2_Inverse = 1.0 / Sqrt_2;
	constexpr double Sqrt_3 = 1.73205080756887729353;
	constexpr double Sqrt_3_Inverse = 1.0 / Sqrt_3;
	constexpr double One_Third = 1.0 / 3.0;
	constexpr double Two_Thirds = 2.0 / 3.0;
}
namespace constd = double_constants;

namespace float_constants {
	constexpr float Tau = 6.28318530717958647693f;
	constexpr float Tau_Inverse = 1.0f / Tau;
	constexpr float Tau_Half = Tau / 2.0f;
	constexpr float Pi = Tau_Half;
	constexpr float Pi_Inverse = 1.0f / Pi;
	constexpr float Pi_Half = Pi / 2.0f;
	constexpr float Sqrt_2 = 1.41421356237309504880f;
	constexpr float Sqrt_2_Inverse = 1.0f / Sqrt_2;
	constexpr float Sqrt_3 = 1.73205080756887729353f;
	constexpr float Sqrt_3_Inverse = 1.0f / Sqrt_3;
	constexpr float One_Third = 1.0f / 3.0f;
	constexpr float Two_Thirds = 2.0f / 3.0f;
}
namespace constf = float_constants;

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
inline const Type& maximum(const Type& a, const Type& b) { return a > b ? a : b; }

// returns the minimum of two given values using the "<" operator
template <typename Type>
inline const Type& minimum(const Type& a, const Type& b) { return a < b ? a : b; }

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

// rounds "value" to the nearest multiple of 1/"n"
inline float round(float value, int n) {
	return round(value * n) / n;
}

// rounds "value" down to the nearest multiple of 1/"n"
inline float floor(float value, int n) {
	return floor(value * n) / n;
}

// rounds "value" up to the nearest multiple of 1/"n"
inline float ceil(float value, int n) {
	return ceil(value * n) / n;
}
