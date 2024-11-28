#include "./include/Edit.h"
#include "./include/Transport.h"

Edit::Edit(TracktionEngine &engine)
    : edit(std::make_unique<te::Edit>(*engine.engine.get(), te::Edit::EditRole::forEditing))
{
}

// Edit::~Edit() = default;

Transport *Edit::getTransport()
{
    if (!edit)
        return nullptr;

    if (!transport)
    {
        transport = std::make_unique<Transport>(edit->getTransport());
    }
    return transport.get();
}

const char *Edit::getName()
{
    if (!edit)
        return "";
    return edit->getName().toRawUTF8();
}

bool Edit::hasChangedSinceSaved() const
{
    return edit ? edit->hasChangedSinceSaved() : false;
}

void Edit::resetChangedStatus()
{
    if (edit)
        edit->resetChangedStatus();
}

Track *Edit::getOrInsertAudioTrackAt(int index)
{
    if (edit)
    {
        auto &trackList = edit->getTrackList();
        if (index >= 0 && index < trackList.size())
        {
            return new Track(*trackList.at(index));
        }
        else
        {
            auto newTrack = edit->insertNewAudioTrack(te::TrackInsertPoint(nullptr, nullptr), nullptr);
            return new Track(*newTrack);
        }
    }
    return nullptr;
}

TempoSequence *Edit::getTempoSequence()
{
    if (!edit)
        return nullptr;

    if (!tempoSequence)
    {
        tempoSequence = std::make_unique<TempoSequence>(edit->tempoSequence);
    }
    return tempoSequence.get();
}