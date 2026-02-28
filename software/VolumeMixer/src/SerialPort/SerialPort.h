#pragma once
#include <windows.h>
#include <string>

class SerialPort
{
protected:
	HANDLE hSerial = INVALID_HANDLE_VALUE;
	std::string buffer;

public:
	SerialPort() = default;
	virtual ~SerialPort();

	bool open(const std::wstring& port, int baudRate = 9600);
	void close();
	bool isOpen() const;

	bool writeLine(const std::string& line);
	bool readLine(std::string& outLine);
	//bool tryReadLine(std::string& outLine);
	//bool readLatestLine(std::string& outLine);

protected:
	bool configure(int baudRate);
};