#pragma once

#include <tracktion_engine/tracktion_engine.h>

namespace te = tracktion;
using namespace std::literals;

namespace AudioEngineHelpers
{

    inline te::Project::Ptr createTempProject(te::Engine &engine)
    {
        auto file = engine.getTemporaryFileManager().getTempDirectory().getChildFile("temp_project").withFileExtension(te::projectFileSuffix);
        te::ProjectManager::TempProject tempProject(engine.getProjectManager(), file, true);
        return tempProject.project;
    }

    template <typename ClipType>
    typename ClipType::Ptr loopAroundClip(ClipType &clip)
    {
        auto &transport = clip.edit.getTransport();
        transport.setLoopRange(clip.getEditTimeRange());
        transport.looping = true;
        transport.setPosition(0s);
        transport.play(false);

        return clip;
    }

    inline te::AudioTrack *getOrInsertAudioTrackAt(te::Edit &edit, int index)
    {
        edit.ensureNumberOfAudioTracks(index + 1);
        return te::getAudioTracks(edit)[index];
    }

    inline void removeAllClips(te::AudioTrack &track)
    {
        auto clips = track.getClips();

        for (int i = clips.size(); --i >= 0;)
            clips.getUnchecked(i)->removeFromParent();
    }

    inline te::WaveAudioClip::Ptr loadAudioFileAsClip(te::Edit &edit, const juce::File &file)
    {
        // Find the first track and delete all clips from it
        if (auto track = getOrInsertAudioTrackAt(edit, 0))
        {
            removeAllClips(*track);

            // Add a new clip to this track
            te::AudioFile audioFile(edit.engine, file);

            if (audioFile.isValid())
                if (auto newClip = track->insertWaveClip(file.getFileNameWithoutExtension(), file,
                                                         {{{}, te::TimeDuration::fromSeconds(audioFile.getLength())}, {}}, false))
                    return newClip;
        }

        return {};
    }
}