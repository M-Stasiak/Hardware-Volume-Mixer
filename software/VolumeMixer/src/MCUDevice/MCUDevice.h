#pragma once
#include "../SerialPort/SerialPort.h"
#include <vector>

class MCUDevice
{
protected:
	SerialPort serial;
	std::vector<int> volumeValues;

public:
	MCUDevice() = default;

	bool connect(const std::wstring& port, int baudRate = 9600);
	void disconnect();

	bool readDataLine();

	const std::vector<int>& getVolumeValues() const;

protected:
	void parseLine(const std::string& line);
};