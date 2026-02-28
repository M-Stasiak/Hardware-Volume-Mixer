#include "AudioController/AudioController.h"
#include "MCUDevice/MCUDevice.h"
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
	MCUDevice device;
	vector<int> volumes;
	if (!device.connect(L"COM3")) return -1;

	while (true) {
		device.readDataLine();
		volumes = device.getVolumeValues();
		for (auto i : volumes) {
			cout << i << " ";
		}
		cout << endl;

		Sleep(2000);

	}

	return 0;
}