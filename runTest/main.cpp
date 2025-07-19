#include <iostream>
#include <fstream>
#include <string>

struct FilePosition {
	int lineNum;
	int linePos;
	size_t strSize;
};

FilePosition findInFile(const std::string& search, std::fstream& file) {
	std::string line;
	FilePosition pos{ 0, 0, search.size() };
	
	while (std::getline(file, line)) {

		pos.linePos = line.find(search);

		if (pos.linePos != std::string::npos) {
			break;
		}

		pos.lineNum++;
	}

	file.seekg(0);
	return pos;
}

std::string getValue(std::fstream& file, FilePosition pos) {
	std::string line;

	for (size_t i = 0; i <= pos.lineNum; i++) {
		std::getline(file, line);
	}
	
	int size = line.find("mWh") - (pos.linePos + pos.strSize + 16);

	std::string value = line.substr(pos.linePos + pos.strSize + 16, size);

	file.seekg(0);
	return value;
}


int main(int argc, char* argv[]) {

	std::string filepath = argv[1];

	std::fstream file;
	file.open(filepath);

	float designCapacity;
	float chargeCapacity;


	if (file.is_open()) {
		FilePosition design = findInFile("DESIGN CAPACITY", file);
		FilePosition charge = findInFile("FULL CHARGE CAPACITY", file);

		std::string designCap = getValue(file, design);
		std::string chargeCap = getValue(file, charge);

		designCap.erase(designCap.find(','), 1);
		chargeCap.erase(chargeCap.find(','), 1);

		designCapacity = std::stoi(designCap);
		chargeCapacity = std::stoi(chargeCap);

		file.close();

		std::cout << "DESIGN CAPACITY:\t" << designCapacity << '\n';
		std::cout << "FULL CHARGE CAPACITY:\t" << chargeCapacity << '\n';

		float batteryWear = 100 - chargeCapacity / designCapacity * 100;

		std::cout << "BATTERY WEAR:\t\t" << batteryWear << '\n';
	}
	else {
		std::cout << "couldn't open file " << filepath << '\n';
	}
}