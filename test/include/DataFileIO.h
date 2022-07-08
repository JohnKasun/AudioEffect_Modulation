#pragma once

#include <string>
#include <fstream>
#include <iostream>

namespace DataFile {
	void printBuffer(const std::string& filename, const float* buffer, const int size);
	void appendToFile(std::ofstream& file, const float* buffer, const int size);
}