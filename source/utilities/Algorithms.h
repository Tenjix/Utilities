#pragma once

#include <algorithm>

template<class Container>
bool begins_with(const Container& match, const Container& scanned) {
	return scanned.size() >= match.size() and std::equal(match.begin(), match.end(), scanned.begin());
}

template<class Container>
bool ends_with(const Container& match, const Container& scanned) {
	return scanned.size() >= match.size() and std::equal(scanned.end() - match.size(), match.end(), match.begin());
}

// copies entries from "source" by iterating in groups of two and emplacing them to the back of "destination" together
template<class SourceContainer, class DestinationContainer>
void copy_tuple(const SourceContainer& source, DestinationContainer& destination) {
	auto range = source.end() - source.begin();
	auto iterator = source.begin();
	while (std::distance(source.begin(), iterator) < range) {
		auto& a = *iterator++;
		auto& b = *iterator++;
		destination.emplace_back(a, b);
	}
}

// copies entries from "source" by iterating in groups of three and emplacing them to the back of "destination" together
template<class SourceContainer, class DestinationContainer>
void copy_triple(const SourceContainer& source, DestinationContainer& destination) {
	auto range = source.end() - source.begin();
	auto iterator = source.begin();
	while (std::distance(source.begin(), iterator) < range) {
		auto& a = *iterator++;
		auto& b = *iterator++;
		auto& c = *iterator++;
		destination.emplace_back(a, b, c);
	}
}

// copies entries from "source" by iterating in groups of four and emplacing them to the back of "destination" together
template<class SourceContainer, class DestinationContainer>
void copy_quadruple(const SourceContainer& source, DestinationContainer& destination) {
	auto range = source.end() - source.begin();
	auto iterator = source.begin();
	while (std::distance(source.begin(), iterator) < range) {
		auto& a = *iterator++;
		auto& b = *iterator++;
		auto& c = *iterator++;
		auto& d = *iterator++;
		destination.emplace_back(a, b, c, d);
	}
}
