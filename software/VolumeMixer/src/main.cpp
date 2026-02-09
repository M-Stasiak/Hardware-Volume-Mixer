#include <iostream>
#include "AudioController/AudioController.h"
using namespace std;

float masterLevel = 0.0f;

int main()
{
	cout << "Uruchomienie AudioController" << endl;

	AudioController audioController;

	masterLevel = audioController.GetMasterVolume();
	cout << "Aktualny level: " << masterLevel*100 << "%" << endl;

	Sleep(1000);

	audioController.SetMasterVolume(0.5f);
	cout << "Ustawienie na: " << masterLevel * 100 << "%" << endl;

	masterLevel = audioController.GetMasterVolume();
	cout << "Aktualny level: " << masterLevel * 100 << "%" << endl;

	return 0;
}