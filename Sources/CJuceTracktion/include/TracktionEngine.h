#pragma once

#include "swift/bridging"
#include "../extra/JuceTracktion/JuceLibraryCode/JuceHeader.h"
#include <memory>

namespace te = tracktion;

class TracktionEngine {
public:
    static TracktionEngine& getInstance() {
        static TracktionEngine instance;
        return instance;
    }
    TracktionEngine(const TracktionEngine&) = delete;
    TracktionEngine& operator=(const TracktionEngine&) = delete;

    void initialize();
    void shutdown();
    const char* getVersion();
    
    te::Engine* getEngine() { return engine.get(); }
     std::unique_ptr<te::Engine> engine;
private:
    TracktionEngine()
        : engine{std::make_unique<te::Engine>("JUCETracktionKit")}
    {
    }

   
} SWIFT_IMMORTAL_REFERENCE;



