#pragma once
#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <audiopolicy.h>

class AudioController
{
private:
	IMMDeviceEnumerator* deviceEnumerator = nullptr;
	IMMDevice* device = nullptr;
	IAudioEndpointVolume* endpointVolume = nullptr;
	IAudioSessionManager2* sessionManager = nullptr;

public:
	AudioController();
	~AudioController();

	void SetMasterVolume(float level);
	float GetMasterVolume();
	//void SetAppVolume();
};