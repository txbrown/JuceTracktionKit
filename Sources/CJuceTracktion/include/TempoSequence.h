#pragma once
#include "swift/bridging"
#include "../extra/JuceTracktion/JuceLibraryCode/JuceHeader.h"
#include <atomic>

namespace te = tracktion;

class Tempo
{
public:
    Tempo(te::TempoSetting &t);
    void setBpm(double bpm);

    void retain()
    {
        refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release()
    {
        if (refCount.fetch_sub(1, std::memory_order_acq_rel) == 1)
        {
            delete this;
        }
    }

private:
    te::TempoSetting &tempo;
    std::atomic<int> refCount{1};

} SWIFT_SHARED_REFERENCE(retainTempo, releaseTempo);

inline void retainTempo(Tempo *tempo)
{
    if (tempo)
        tempo->retain();
}

inline void releaseTempo(Tempo *tempo)
{
    if (tempo)
        tempo->release();
}

class TempoSequence
{
public:
    TempoSequence(te::TempoSequence &seq) : sequence(seq) {}

    Tempo *getTempo(int index) SWIFT_RETURNS_INDEPENDENT_VALUE
    {
        return new Tempo(*sequence.getTempo(index));
    }

    double barsBeatsToTime(int bars, double beats)
    {
        auto barsAndBeats = te::tempo::BarsAndBeats{bars, te::BeatDuration::fromBeats(beats)};
        auto timePos = sequence.barsBeatsToTime(barsAndBeats);
        return timePos.inSeconds();
    }

    void retain()
    {
        refCount.fetch_add(1, std::memory_order_relaxed);
    }

    void release()
    {
        if (refCount.fetch_sub(1, std::memory_order_acq_rel) == 1)
        {
            delete this;
        }
    }

private:
    te::TempoSequence &sequence;
    std::atomic<int> refCount{1};
} SWIFT_SHARED_REFERENCE(retainTempoSequence, releaseTempoSequence);

inline void retainTempoSequence(TempoSequence *tempoSequence)
{
    if (tempoSequence)
        tempoSequence->retain();
}

inline void releaseTempoSequence(TempoSequence *tempoSequence)
{
    if (tempoSequence)
        tempoSequence->release();
}