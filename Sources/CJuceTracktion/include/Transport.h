#pragma once

#include "swift/bridging"
#include "../extra/JuceTracktion/JuceLibraryCode/JuceHeader.h"
#include <atomic>

namespace te = tracktion;

class Transport
{
public:
    explicit Transport(te::TransportControl &t);

    void play(bool shouldPlay);
    void stop(bool discardRecordings, bool rewindToStart);

    double getCurrentPosition() const;
    void setPosition(double newPosition);

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
    te::TransportControl &transport;
    std::atomic<int> refCount{1};
} SWIFT_SHARED_REFERENCE(retainTransport, releaseTransport);

inline void retainTransport(Transport *transport)
{
    if (transport)
        transport->retain();
}

inline void releaseTransport(Transport *transport)
{
    if (transport)
        transport->release();
}
