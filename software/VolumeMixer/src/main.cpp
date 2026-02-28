#include "AudioController/AudioController.h"
#include "SerialPort/SerialPort.h"
#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

float masterLevel = 0.0f;

int main()
{
	/*
	cout << "Uruchomienie AudioController" << endl;

	AudioController audioController;

	for (int i = 0; i <= 100; i++) {
		float level = (float)i / 100;
		audioController.SetAppVolume(L"Spotify.exe", level);
		std::cout << audioController.GetAppVolume(L"Spotify.exe") << std::endl;
		Sleep(100);
	}
	*/
	/*
	masterLevel = audioController.GetMasterVolume();
	cout << "Aktualny level: " << masterLevel*100 << "%" << endl;

	Sleep(1000);

	audioController.SetMasterVolume(0.4f);
	cout << "Ustawienie na: " << masterLevel * 100 << "%" << endl;

	masterLevel = audioController.GetMasterVolume();
	cout << "Aktualny level: " << masterLevel * 100 << "%" << endl;

	Sleep(1000);

	audioController.SetAppVolume(L"Spotify.exe", 0.5f);
	cout << "Ustawienie na: " << masterLevel * 100 << "%" << endl;
	*/

	AudioController audioController;
	SerialPort port;
	if (!port.open(L"COM3")) return -1;

	while (true) {
		std::string line;
		cout << port.isOpen() << " - ";
		if (port.readLine(line)) {
			vector<int> values;
			stringstream ss(line);
			string item;

			while (getline(ss, item, '|')) {
				if (item.empty()) continue;

				try {
					int value = stoi(item);
					values.push_back(value);
				}
				catch (const invalid_argument&) {
					values.push_back(0);
				}
				catch (const out_of_range&) {
					values.push_back(0);
				}
			}
			for (size_t i = 0; i < values.size(); i++) cout << "A" << i << " = " << values[i] << "\t";
			audioController.SetMasterVolume(values.front() / 100.0f);
		}
		cout << endl;
	}

	return 0;
}