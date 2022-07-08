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

void appendToFile(std::ofstream& file, const auto* buffer, const int size) {
	if (file) {
		file << "\n";
		for (int i = 0; i < size; i++) {
			file << buffer[i] << ",";
		}
	}
}

TEST_CASE("Print Wavetables") {
	static const auto size = 100;
	float buffer[size]{};
	std::ofstream out_file{};
	out_file.open("C:/Users/JohnK/Documents/ASE/AudioEffect_Chorus/build/Triangle.csv");
	Wavetable::generateTri(buffer, size, 2);
	appendToFile(out_file, buffer, size);
	Wavetable::generateSine(buffer, size, 2);
	appendToFile(out_file, buffer, size);
	out_file.close();
}