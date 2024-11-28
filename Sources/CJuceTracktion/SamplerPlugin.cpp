#include "./include/SamplerPlugin.h"
#include "../extra/JuceTracktion/JuceLibraryCode/JuceHeader.h"

SamplerPlugin::SamplerPlugin(te::SamplerPlugin& p) : plugin(p) {}

void SamplerPlugin::setSoundMedia(int channel, const char* path) {
    plugin.setSoundMedia(channel, juce::String(path));
}

void SamplerPlugin::setChannelVolume(int channel, float volume) {
    plugin.setSoundGains(channel, volume, 0);
}

int SamplerPlugin::getNumChannels() const {
    return plugin.getNumSounds();
} 