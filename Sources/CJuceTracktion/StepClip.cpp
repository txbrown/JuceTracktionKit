#include "./include/StepClip.h"

Pattern::Pattern(te::StepClip::Pattern& p) : pattern(p) {}

void Pattern::clear() {
    pattern.clear();
}

void Pattern::randomiseSteps() {
    pattern.randomiseSteps();
}

void Pattern::setNote(int channel, int step, bool enabled) {
    pattern.setNote(channel, step, enabled);
}

StepClip::StepClip(te::StepClip& c) : clip(c) {}

Pattern* StepClip::pattern(int index) {
    te::StepClip::Pattern p = clip.getPattern(index);
    return new Pattern(p);
}

int StepClip::getNumChannels() const {
    return clip.getChannels().size();
}

int StepClip::getSteps() {
    te::StepClip::Pattern p = clip.getPattern(0);
    return p.getNumNotes();
}

int StepClip::getNumPatterns() const {
    return clip.getPatterns().size();
} 