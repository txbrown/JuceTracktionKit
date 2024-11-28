#pragma once
#include "swift/bridging"
#include "../extra/JuceTracktion/JuceLibraryCode/JuceHeader.h"
#include "Transport.h"
#include "TempoSequence.h"
#include "Track.h"
#include "PluginCache.h"
#include "TracktionEngine.h"
#include <atomic>

namespace te = tracktion;
class Edit
{
public:
    Edit(TracktionEngine &engine);
    Edit(const Edit &) = delete; // non-copyable

    static Edit *create(TracktionEngine &engine)
    {
        return new Edit(engine);
    }

    const char *getName();
    bool hasChangedSinceSaved() const;
    void resetChangedStatus();

    Transport *getTransport();
    TempoSequence *getTempoSequence();
    Track *getOrInsertAudioTrackAt(int index);
    PluginCache *getPluginCache();

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
    std::unique_ptr<te::Edit> edit;
    std::unique_ptr<Transport> transport;
    std::unique_ptr<TempoSequence> tempoSequence;
    std::atomic<int> refCount{1};
} SWIFT_SHARED_REFERENCE(retainEdit, releaseEdit);

inline void retainEdit(Edit *edit)
{
    if (edit)
        edit->retain();
}

inline void releaseEdit(Edit *edit)
{
    if (edit)
        edit->release();
}
