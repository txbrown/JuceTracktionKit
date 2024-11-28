#pragma once
#include "swift/bridging"
#include "../extra/JuceTracktion/JuceLibraryCode/JuceHeader.h"
#include "EditTimeRange.h"

namespace te = tracktion;

template <typename T>
class Array
{
public:
    Array() = default;
    void add(T item);
    T operator[](int index);
    int size() const;

private:
    std::vector<T> items;
};

class PluginList
{
public:
    PluginList(te::Plugin::Array &list);
    void insertPlugin(te::Plugin &plugin, int index);

private:
    te::Plugin::Array &plugins;
} SWIFT_SELF_CONTAINED;

enum class ClipType
{
    step = 0,
    midi = 1,
    audio = 2,
};

class Clip
{
public:
    Clip(te::Clip &c) : clip(c) {}

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
    te::Clip &clip;
    std::atomic<int> refCount{1};
} SWIFT_SHARED_REFERENCE(retainClip, releaseClip);

inline void retainClip(Clip *clip)
{
    if (clip)
        clip->retain();
}

inline void releaseClip(Clip *clip)
{
    if (clip)
        clip->release();
}

class Track
{
public:
    Track(te::Track &track)
        : track(track) {}

    PluginList *getPluginList();
    void insertNewClip(ClipType type, const char *name, EditTimeRange &timeRange)
    {
        if (auto *clipTrack = dynamic_cast<te::ClipTrack *>(&track))
        {
            te::TimeRange range(te::TimePosition::fromSeconds(0.0),
                                te::TimePosition::fromSeconds(4.0));
            te::ClipPosition position{range, {}};
            clipTrack->insertNewClip(static_cast<te::TrackItem::Type>(type),
                                     juce::String(name),
                                     position,
                                     nullptr);
        }
    }

    Array<Clip *> *getClips()
    {
        auto *clips = new Array<Clip *>();
        if (auto *clipTrack = dynamic_cast<te::ClipTrack *>(&track))
        {
            for (auto *clip : clipTrack->getClips())
            {
                clips->add(new Clip(*clip));
            }
        }
        return clips;
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
    te::Track &track;
    std::atomic<int> refCount{1};
} SWIFT_SHARED_REFERENCE(retainEdit, releaseEdit);

inline void retainTrack(Track *track)
{
    if (track)
        track->retain();
}

inline void releaseTrack(Track *track)
{
    if (track)
        track->release();
}