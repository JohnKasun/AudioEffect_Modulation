#pragma once

#include <iostream>
#include <fstream>

#include "catch.hpp"
#include "CatchUtil.h"
#include "Wavetable.h"

void printBuffer(const std::string& filename, const auto* buffer, const int size) {
	std::ofstream out_file{};
	out_file.open(filename);
	if (out_file) {
		for (int i = 0; i < size; i++) {
			out_file << buffer[i] << ",";
		}
	}
	out_file.close();
}

TEST_CASE("Print Triangle") {
	static const auto size = 100;
	float buffer[size]{};
	Wavetable::generateTri(buffer, size, 2);
	printBuffer("C:/Users/JohnK/Documents/ASE/AudioEffect_Chorus/build/Triangle.csv", buffer, size);
}