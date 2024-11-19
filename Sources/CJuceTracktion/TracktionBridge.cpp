#include "include/TracktionBridge.h"
#include "extra/JuceTracktion/JuceLibraryCode/JuceHeader.h"

TracktionEngineBridge::TracktionEngineBridge() {
    engine = new tracktion::engine::Engine("JuceTracktionKit");
}

TracktionEngineBridge::~TracktionEngineBridge() {
    delete engine;
}

void TracktionEngineBridge::initialize() {
}

void TracktionEngineBridge::shutdown() {
  delete engine;
}
