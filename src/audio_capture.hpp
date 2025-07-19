#pragma once
#include <vector>

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
    static class IAudioCaptureClient* pCaptureClient;
    static class IAudioClient* pAudioClient;
    static bool capturing;
};