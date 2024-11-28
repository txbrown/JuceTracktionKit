#pragma once
#include "swift/bridging"
#include "../extra/JuceTracktion/JuceLibraryCode/JuceHeader.h"
#include "SamplerPlugin.h"


class PluginCache {
public:
    PluginCache(te::PluginCache& cache);
    
    te::Plugin* createPlugin(te::Plugin::Type type);
    
private:
    te::PluginCache& cache;
} SWIFT_SELF_CONTAINED; 