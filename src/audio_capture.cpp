// Code for WASAPI loopback recording
// Needed to run FFT on live audio streams
#define NOMINMAX

#include "audio_capture.hpp"

// NOMINMAX is to stop Windows.h colliding with std::min
#include <Windows.h>

#include <mmdeviceapi.h>
#include <audioclient.h>
#include <wrl/client.h>
#include <iostream>
#include <algorithm>

#pragma comment(lib, "Ole32.lib")

using Microsoft::WRL::ComPtr;

ComPtr<IAudioCaptureClient> AudioCapture::pCaptureClient;
ComPtr<IAudioClient> AudioCapture::pAudioClient;
bool AudioCapture::capturing = false;

bool AudioCapture::initialize() {
    std::ignore = CoInitializeEx(NULL, COINIT_MULTITHREADED);

    ComPtr<IMMDeviceEnumerator> pEnumerator;
    ComPtr<IMMDevice> pDevice;

    HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL,
        IID_PPV_ARGS(&pEnumerator));
    if (FAILED(hr)) return false;

    hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
    if (FAILED(hr)) return false;

    hr = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, &pAudioClient);
    if (FAILED(hr)) return false;

    WAVEFORMATEX* pwfx = nullptr;
    hr = pAudioClient->GetMixFormat(&pwfx);
    if (FAILED(hr)) return false;

    REFERENCE_TIME bufferDuration = 10000000; // 1 second
    hr = pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED,
        AUDCLNT_STREAMFLAGS_LOOPBACK,
        bufferDuration, 0, pwfx, nullptr);
    CoTaskMemFree(pwfx); // release audio format memory
    if (FAILED(hr)) return false;

    hr = pAudioClient->GetService(IID_PPV_ARGS(&pCaptureClient));
    return SUCCEEDED(hr);

}

bool AudioCapture::start() {
    HRESULT hr = pAudioClient->Start();
    capturing = SUCCEEDED(hr);
    return capturing;
}

bool AudioCapture::stop() {
    HRESULT hr = pAudioClient->Stop();
    capturing = false;
    return SUCCEEDED(hr);
}

std::vector<float> AudioCapture::getSamples() {
    std::vector<float> samples;

    BYTE* pData;
    UINT32 numFrames;
    DWORD flags;

    HRESULT hr = pCaptureClient->GetBuffer(&pData, &numFrames, &flags, NULL, NULL);
    if (SUCCEEDED(hr)) {
        float* floatData = reinterpret_cast<float*>(pData);
        for (UINT32 i = 0; i < numFrames; ++i)
            samples.push_back(floatData[i]);

        pCaptureClient->ReleaseBuffer(numFrames);
    }

    return samples;
}

float AudioCapture::calculateEnergy() {
    auto samples = getSamples();
    if (samples.empty()) return 0.0f;

    float energy = 0.0f;
    for (float sample : samples)
        energy += sample * sample;

    return energy / samples.size();
}

void AudioCapture::debugPrint() {
    auto samples = getSamples();
    if (!samples.empty()) {
        std::cout << "Captured " << samples.size() << " samples. First 5:\n";
        for (size_t i = 0; i < std::min(samples.size(), size_t(5)); ++i)
            std::cout << samples[i] << " ";
        std::cout << std::endl;
    }
    else {
        std::cout << "No samples captured.\n";
    }
}