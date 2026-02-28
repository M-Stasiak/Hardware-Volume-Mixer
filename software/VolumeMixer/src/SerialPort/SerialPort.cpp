#include "SerialPort.h"
#include <iostream>
#include <chrono>

SerialPort::~SerialPort() {
	close();
}

bool SerialPort::open(const std::wstring& port, int baudRate) {
	std::wstring fullPort = (port.substr(0, 4) == L"\\\\.\\") ? port : L"\\\\.\\" + port;

	hSerial = CreateFileW(
		fullPort.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		nullptr,
		OPEN_EXISTING,
		0,
		nullptr
	);

	if (hSerial == INVALID_HANDLE_VALUE) {
		std::wcerr << L"Cannot open port " << port << L"\n";
		return false;
	}
	if (!configure(baudRate)) {
		close();
		return false;
	}

	PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);

	Sleep(2000);

	return true;
}

bool SerialPort::configure(int baudRate) {
	DCB dcb = { 0 };
	dcb.DCBlength = sizeof(dcb);

	if (!GetCommState(hSerial, &dcb)) return false;

	dcb.BaudRate = baudRate;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;
	dcb.Parity = NOPARITY;

	dcb.fBinary = TRUE;
	dcb.fParity = FALSE;

	dcb.fDtrControl = DTR_CONTROL_ENABLE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;

	if (!SetCommState(hSerial, &dcb)) return false;

	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 50;

	if (!SetCommTimeouts(hSerial, &timeouts)) return false;

	return true;
}

void SerialPort::close() {
	if (hSerial == INVALID_HANDLE_VALUE) return;

	PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
	if (!CloseHandle(hSerial)) std::cerr << "CloseHandle failed\n";
	hSerial = INVALID_HANDLE_VALUE;
}

bool SerialPort::isOpen() const {
	return hSerial != INVALID_HANDLE_VALUE;
}

bool SerialPort::writeLine(const std::string& line) {
	if (!isOpen()) return false;

	DWORD errors;
	COMSTAT status;

	if (!ClearCommError(hSerial, &errors, &status)) return false;

	DWORD bytesWritten = 0;
	std::string data = line + "\r\n";

	bool writeStatus = WriteFile(
		hSerial,
		data.c_str(),
		static_cast<DWORD>(data.size()),
		&bytesWritten,
		nullptr
	);

	if (!writeStatus) return false;
	return bytesWritten == data.size();
}

//bool SerialPort::readLine(std::string& outLine) {
//	if (!isOpen()) return false;
//
//	char ch;
//	DWORD bytesRead;
//	DWORD errors;
//	COMSTAT status;
//
//	auto start = std::chrono::steady_clock::now();
//	const int timeoutMs = 2000;
//
//	while (true) {
//		if (!ClearCommError(hSerial, &errors, &status)) return false;
//		if (!ReadFile(hSerial, &ch, 1, &bytesRead, nullptr)) return false;
//
//		if (bytesRead == 0) {
//			if (std::chrono::steady_clock::now() - start > std::chrono::milliseconds(timeoutMs)) return false;
//			else continue;
//		}
//
//		if (ch == '\n') {
//			if (!buffer.empty()) {
//				outLine = buffer;
//				buffer.clear();
//				return true;
//			}
//		}
//		else buffer += ch;
//	}
//}

//bool SerialPort::tryReadLine(std::string& outLine) {
//	if (!isOpen()) return false;
//
//	DWORD errors;
//	COMSTAT status;
//
//	if (!ClearCommError(hSerial, &errors, &status)) return false;
//
//	DWORD toRead = status.cbInQue;
//	if (toRead == 0) return false;
//
//	char* temp = new char[toRead];
//	DWORD bytesRead;
//
//	if (!ReadFile(hSerial, temp, toRead, &bytesRead, nullptr)) {
//		delete[] temp;
//		return false;
//	}
//
//	buffer.append(temp, bytesRead);
//	delete[] temp;
//
//	size_t pos = buffer.find('\n');
//	if (pos != std::string::npos) {
//		outLine = buffer.substr(0, pos);
//		if (!outLine.empty() && outLine.back() == '\r') outLine.pop_back();
//
//		buffer.erase(0, pos + 1);
//		return true;
//	}
//}

bool SerialPort::readLine(std::string& outLine) {
	if (!isOpen())
		return false;

	DWORD errors;
	COMSTAT status;

	if (!ClearCommError(hSerial, &errors, &status))
		return false;

	DWORD toRead = status.cbInQue;
	if (toRead == 0)
		return false;

	char* temp = new char[toRead];
	DWORD bytesRead;

	if (!ReadFile(hSerial, temp, toRead, &bytesRead, nullptr)) {
		delete[] temp;
		return false;
	}

	buffer.append(temp, bytesRead);
	delete[] temp;

	size_t pos = 0;
	size_t lastPos = std::string::npos;

	while ((pos = buffer.find('\n', pos)) != std::string::npos) {
		lastPos = pos;
		pos++;
	}

	if (lastPos != std::string::npos) {
		size_t start = buffer.rfind('\n', lastPos - 1);
		start = (start == std::string::npos) ? 0 : start + 1;

		outLine = buffer.substr(start, lastPos - start);
		if (!outLine.empty() && outLine.back() == '\r')
			outLine.pop_back();

		buffer.erase(0, lastPos + 1);
		return true;
	}

	return false;
}