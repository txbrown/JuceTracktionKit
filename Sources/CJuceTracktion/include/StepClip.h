#pragma once

#include "../extra/JuceTracktion/JuceLibraryCode/JuceHeader.h"

namespace te = tracktion;

class Pattern {
public:
    Pattern(te::engine::StepClip::Pattern& p);
    void clear();
    void randomiseSteps();
    void setNote(int channel, int step, bool enabled);
    
private:
    te::engine::StepClip::Pattern& pattern;
};

class StepClip {
public:
    StepClip(te::engine::StepClip& c);
    Pattern* pattern(int index);
    int getNumChannels() const;
    int getSteps();
    int getNumPatterns() const;
    
private:
    te::engine::StepClip& clip;
}; 