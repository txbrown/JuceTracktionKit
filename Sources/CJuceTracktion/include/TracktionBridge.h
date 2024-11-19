#pragma once
#include "../extra/JuceTracktion/JuceLibraryCode/JuceHeader.h"

class TracktionEngineBridge {
public:
    TracktionEngineBridge();
    ~TracktionEngineBridge();
    
    void initialize();
    void shutdown();

private:
    tracktion::engine::Engine* engine = nullptr;
};
