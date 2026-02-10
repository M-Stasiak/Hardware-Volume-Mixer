#include <iostream>
#include "AudioController/AudioController.h"
using namespace std;

float masterLevel = 0.0f;

int main()
{
	cout << "Uruchomienie AudioController" << endl;

	AudioController audioController;

	for (int i = 0; i <= 100; i++) {
		float level = (float)i / 100;
		audioController.SetAppVolume(L"Spotify.exe", level);
		std::cout << audioController.GetAppVolume(L"Spotify.exe") << std::endl;
		Sleep(100);
	}
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

	return 0;
}