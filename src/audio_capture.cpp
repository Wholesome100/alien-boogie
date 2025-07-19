// Code for WASAPI loopback recording
// Needed to run FFT on live audio streams

#include "audio_capture.hpp"

#define NOMINMAX
#include <Windows.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#include <iostream>
#include <algorithm>

#pragma comment(lib, "Ole32.lib")

IAudioCaptureClient* AudioCapture::pCaptureClient = nullptr;
IAudioClient* AudioCapture::pAudioClient = nullptr;
bool AudioCapture::capturing = false;

bool AudioCapture::initialize() {
    std::ignore = CoInitializeEx(NULL, COINIT_MULTITHREADED);

    IMMDeviceEnumerator* pEnumerator = nullptr;
    IMMDevice* pDevice = nullptr;
    std::ignore = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, IID_PPV_ARGS(&pEnumerator));
    pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);

    pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&pAudioClient);

    WAVEFORMATEX* pwfx = nullptr;
    pAudioClient->GetMixFormat(&pwfx);

    REFERENCE_TIME hnsBufferDuration = 10000000; // 1 second
    pAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED,
        AUDCLNT_STREAMFLAGS_LOOPBACK,
        hnsBufferDuration, 0, pwfx, NULL);

    pAudioClient->GetService(IID_PPV_ARGS(&pCaptureClient));
    return true;
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