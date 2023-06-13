#include "Utils.hpp"

#include <fstream>
#include <iostream>

bool Utils::ReadFile(const std::string& filename, List<uint8>& output) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
		throw std::runtime_error("Utils::ReadFile(): Failed to open file");

	output.Resize((uint32)file.tellg());

	file.seekg(0);
	file.read((char*)output.Data(), output.Length());
	file.close();

	return true;
}
