#include "DataFileIO.h"

void DataFile::printBuffer(const std::string& filename, const float* buffer, const int size) {
	std::ofstream out_file{};
	out_file.open(filename);
	if (out_file) {
		for (int i = 0; i < size; i++) {
			out_file << buffer[i] << ",";
		}
	}
	out_file.close();
}

void DataFile::appendToFile(std::ofstream& file, const float* buffer, const int size) {
	if (file) {
		file << "\n";
		for (int i = 0; i < size; i++) {
			file << buffer[i] << ",";
		}
	}
}