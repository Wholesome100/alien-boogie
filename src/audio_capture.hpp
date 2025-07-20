#pragma once
#include <vector>

#include <mmdeviceapi.h>
#include <audioclient.h>
#include <wrl/client.h> // For ComPtr


class AudioCapture {
public:
    bool initialize();
    bool start();
    bool stop();

    std::vector<float> getSamples();
    float calculateEnergy();
    void debugPrint();

private:
    // No external access needed
    // COM interface wrappers
    static Microsoft::WRL::ComPtr<IAudioCaptureClient> pCaptureClient;
    static Microsoft::WRL::ComPtr<IAudioClient> pAudioClient;

    static bool capturing;
};
