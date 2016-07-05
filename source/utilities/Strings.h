#pragma once

#include <codecvt>
#include <sstream>

#include <utilities/Standard.h>

using StringConverter = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>;

// creates a String of the given arguments using the ostream "<<" operator
template <typename... Arguments>
String stringify(const Arguments&... arguments) {
	std::ostringstream stream;
	for_each_variadic(stream << arguments);
	return stream.str();
}

// converts arithmetic types to string using std::to_string
template <class Type, typename std::enable_if<std::is_arithmetic<Type>::value>::type* = nullptr>
String to_string(const Type& value) {
	return std::to_string(value);
}

// converts non-arithmetic types to string using stringstream
template <class Type, typename std::enable_if<not std::is_arithmetic<Type>::value>::type* = nullptr>
String to_string(const Type& value) {
	std::ostringstream stream;
	stream << value;
	return stream.str();
}

// determines whether "scanned" begins with "match"
inline bool begins_with(const String& match, const String& scanned) {
	return scanned.size() >= match.size() and scanned.compare(0, match.length(), match) == 0;
}

// determines whether "scanned" ends with "match"
inline bool ends_with(const String& match, const String& scanned) {
	return scanned.size() >= match.size() and scanned.compare(scanned.size() - match.size(), match.size(), match) == 0;
}

// determines whether "scanned" contains "match"
inline bool contains(const String& match, const String& scanned) {
	return scanned.find(match) != String::npos;
}

// determines whether "a" equals "b"
// (returns true if "a" and "b" are nullpointers, false if only one is a nullpointer)
inline bool equals(const char* a, const char* b) {
	if (a == nullptr and b == nullptr) return true;
	if (a == nullptr or b == nullptr) return false;
	return strcmp(a, b) == 0;
}

// determines whether "scanned" begins with "match"
// (returns false if "match" or "scanned" are nullpointers)
inline bool begins_with(const char* match, const char* scanned) {
	if (match == nullptr or scanned == nullptr) return false;
	uint match_length = strlen(match);
	uint scanned_length = strlen(scanned);
	if (scanned_length < match_length) return false;
	return strncmp(match, scanned, match_length) == 0;
}

// determines whether "scanned" ends with "match"
// (returns false if "match" or "scanned" are nullpointers)
inline bool ends_with(const char* match, const char* scanned) {
	if (match == nullptr or scanned == nullptr) return false;
	uint match_length = strlen(match);
	uint scanned_length = strlen(scanned);
	if (scanned_length < match_length) return false;
	const char* match_in_scanned = strstr(scanned, match);
	uint position_in_scanned = match_in_scanned - scanned;
	return position_in_scanned == scanned_length - match_length;
}

// determines whether "scanned" contains "match"
// (returns false if "match" or "scanned" are nullpointers)
inline bool contains(const char* match, const char* scanned) {
	if (match == nullptr or scanned == nullptr) return false;
	const char* match_in_scanned = strstr(scanned, match);
	return match_in_scanned != nullptr;
}

// converts an utf8 string to an utf16 wide-string
inline std::wstring to_utf16(const String& utf8_string) {
	return StringConverter().from_bytes(utf8_string);
}

// converts an utf8 string to an utf16 wide-string
inline std::wstring to_utf16(const char* utf8_string) {
	return StringConverter().from_bytes(utf8_string);
}

// converts an utf16 wide-string to an utf8 string
inline String to_utf8(const std::wstring& utf16_string) {
	return StringConverter().to_bytes(utf16_string);
}

// converts an utf16 wide-string to an utf8 string
inline String to_utf8(const wchar_t* utf16_string) {
	return StringConverter().to_bytes(utf16_string);
}

// returns the path substring from "file_name"
inline String get_path(const String& file_name) {
	uint last_slash_pos = file_name.find_last_of("/\\");
	if (last_slash_pos != String::npos) return file_name.substr(0, last_slash_pos + 1);
	return "";
}
