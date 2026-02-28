#include "../MCUDevice/MCUDevice.h"
#include <iostream>
#include <sstream>
#include <algorithm>

bool MCUDevice::connect(const std::wstring& port, int baudRate) {
	return serial.open(port, baudRate);
}

void MCUDevice::disconnect() {
	serial.close();
}

bool MCUDevice::readDataLine() {
	std::string line;
	if (serial.readLine(line)) {
		parseLine(line);
		return true;
	}
	return false;
}

void MCUDevice::parseLine(const std::string& line) {

	volumeValues.clear();

	std::stringstream ss(line);
	std::string item;

	while (std::getline(ss, item, '|')) {
		if (item.empty()) continue;

		try {
			int value = std::clamp(std::stoi(item), 0, 100);
			volumeValues.push_back(value);
		}
		catch (...) {
			volumeValues.push_back(0);
		}
	}
}

const std::vector<int>& MCUDevice::getVolumeValues() const {
	return volumeValues;
}