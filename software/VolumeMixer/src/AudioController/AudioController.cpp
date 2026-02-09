#include "AudioController.h"

AudioController::AudioController() {
	CoInitialize(nullptr);

	CoCreateInstance(
		__uuidof(MMDeviceEnumerator),
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&deviceEnumerator)
	);

	deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
	
	device->Activate(
		__uuidof(IAudioEndpointVolume),
		CLSCTX_INPROC_SERVER,
		nullptr,
		(void**)&endpointVolume
	);

	device->Activate(
		__uuidof(IAudioSessionManager2),
		CLSCTX_INPROC_SERVER,
		nullptr,
		(void**)&sessionManager
	);

}

AudioController::~AudioController() {
	if (sessionManager) sessionManager->Release();
	if (endpointVolume) endpointVolume->Release();
	if (device) device->Release();
	if (deviceEnumerator) deviceEnumerator->Release();
	CoUninitialize();
}

void AudioController::SetMasterVolume(float level) {
	if (endpointVolume) {
		if (level < 0.0f) level = 0.0f;
		if (level > 1.0f) level = 1.0f;

		endpointVolume->SetMasterVolumeLevelScalar(level, nullptr);
	}
}

float AudioController::GetMasterVolume() {
	float level = 0.0f;
	if (endpointVolume) {
		endpointVolume->GetMasterVolumeLevelScalar(&level);
	}
	return level;
}