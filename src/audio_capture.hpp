#pragma once
#include <vector>

class AudioCapture {
public:
    bool initialize();
    bool start();
    bool stop();
    std::vector<float> getSamples();
    void debugPrint();
};