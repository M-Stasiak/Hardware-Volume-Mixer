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

void AudioController::SetAppVolume(const std::wstring& targetProcessName, float level) {
	if (!sessionManager) return;
	IAudioSessionEnumerator* sessionEnumerator = nullptr;
	sessionManager->GetSessionEnumerator(&sessionEnumerator);
	if (!sessionEnumerator) return;

	int sessionCount;
	sessionEnumerator->GetCount(&sessionCount);

	for (int i = 0; i < sessionCount; i++) {
		IAudioSessionControl* sessionControl = nullptr;
		sessionEnumerator->GetSession(i, &sessionControl);

		IAudioSessionControl2* sessionControl2 = nullptr;
		sessionControl->QueryInterface(
			__uuidof(IAudioSessionControl2),
			(void**)&sessionControl2
		);

		DWORD pid;
		sessionControl2->GetProcessId(&pid);

		HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
		if (hProcess) {
			wchar_t processName[MAX_PATH];
			DWORD size = MAX_PATH;

			QueryFullProcessImageNameW(
				hProcess,
				0,
				processName,
				&size
			);

			std::wstring name(processName);
			if (name.find(targetProcessName) != std::wstring::npos) {
				ISimpleAudioVolume* volumeControl = nullptr;
				sessionControl2->QueryInterface(
					__uuidof(ISimpleAudioVolume),
					(void**)&volumeControl
				);

				if (level < 0.0f) level = 0.0f;
				if (level > 1.0f) level = 1.0f;

				volumeControl->SetMasterVolume(level, nullptr);
				volumeControl->Release();
			}

			CloseHandle(hProcess);
		}

		sessionControl2->Release();
		sessionControl->Release();
	}

	sessionEnumerator->Release();
}

float AudioController::GetAppVolume(const std::wstring& targetProcessName) {

	float resultVolume = -1.0f;

	if (!sessionManager) return -1.0f;
	IAudioSessionEnumerator* sessionEnumerator = nullptr;
	sessionManager->GetSessionEnumerator(&sessionEnumerator);
	if (!sessionEnumerator) return -1.0f;

	int sessionCount;
	sessionEnumerator->GetCount(&sessionCount);

	for (int i = 0; i < sessionCount; i++) {
		IAudioSessionControl* sessionControl = nullptr;
		sessionEnumerator->GetSession(i, &sessionControl);

		IAudioSessionControl2* sessionControl2 = nullptr;
		sessionControl->QueryInterface(
			__uuidof(IAudioSessionControl2),
			(void**)&sessionControl2
		);

		DWORD pid;
		sessionControl2->GetProcessId(&pid);

		HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
		if (hProcess) {
			wchar_t processName[MAX_PATH];
			DWORD size = MAX_PATH;

			QueryFullProcessImageNameW(
				hProcess,
				0,
				processName,
				&size
			);

			std::wstring name(processName);
			if (name.find(targetProcessName) != std::wstring::npos) {
				ISimpleAudioVolume* volumeControl = nullptr;
				sessionControl2->QueryInterface(
					__uuidof(ISimpleAudioVolume),
					(void**)&volumeControl
				);

				volumeControl->GetMasterVolume(&resultVolume);
				volumeControl->Release();
			}

			CloseHandle(hProcess);
		}

		sessionControl2->Release();
		sessionControl->Release();
	}

	return resultVolume;
}