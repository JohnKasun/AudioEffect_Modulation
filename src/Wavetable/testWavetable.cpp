#pragma once

#include "catch.hpp"
#include "CatchUtil.h"
#include "DataFileIO.h"
#include "Wavetable.h"


TEST_CASE("Print Wavetables") {
	static const auto size = 100;
	float buffer[size]{};
	std::ofstream out_file{};
	out_file.open("C:/Users/JohnK/Documents/ASE/AudioEffect_Chorus/build/Wavetables.csv");
	Wavetable::generateTri(buffer, size, 2);
	DataFile::appendToFile(out_file, buffer, size);
	Wavetable::generateSine(buffer, size, 2);
	DataFile::appendToFile(out_file, buffer, size);
	out_file.close();
}