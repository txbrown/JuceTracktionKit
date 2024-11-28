#pragma once

#include "../extra/JuceTracktion/JuceLibraryCode/JuceHeader.h"

namespace te = tracktion;

class SamplerPlugin {
public:
    SamplerPlugin(te::SamplerPlugin& p);
    void setChannelVolume(int channel, float volume);
    void setSoundMedia(int channel, const char* path);
    int getNumChannels() const;
    
private:
    te::SamplerPlugin& plugin;
}; 