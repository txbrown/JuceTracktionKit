#include "TrackManager.h"
#include <iostream>
#include <juce_core/juce_core.h>

TrackManager::TrackManager(te::Edit* edit) : edit(edit) {}

TrackManager::~TrackManager() = default;

int TrackManager::createAudioTrack(const std::string& name) {
  if (!edit)
    return -1;

  te::TrackInsertPoint insertPoint = te::TrackInsertPoint::getEndOfTracks(*edit);

  auto newTrack = edit->insertNewAudioTrack(insertPoint, nullptr);
  if (!newTrack) {
    std::cerr << "Failed to create audio track" << std::endl;
    return -1;
  }

  newTrack->setName(name);

  // Store in the track map
  int trackID = nextTrackID++;
  tracks[trackID] = newTrack;

  return newTrack.get()->itemID.getRawID();
}

bool TrackManager::removeTrack(int trackID) {
  auto it = tracks.find(trackID);
  if (it == tracks.end())
    return false;

  edit->deleteTrack(it->second.get());
  tracks.erase(it);

  return true;
}

bool TrackManager::addAudioClip(int trackID,
                                const std::string& filePath,
                                double startBar,
                                double lengthInBars) {
  // Validate trackID
  auto it = tracks.find(trackID);
  if (it == tracks.end())
    return false;

  // Ensure the track is an AudioTrack
  auto audioTrack = dynamic_cast<te::AudioTrack*>(it->second.get());
  if (!audioTrack)
    return false;

  // Convert bars to time using the TempoSequence
  auto& tempoSequence = edit->tempoSequence;
  te::TimePosition startPosition = te::toTime(te::BeatPosition::fromBeats(startBar), tempoSequence);
  te::TimePosition endPosition =
      te::toTime(te::BeatPosition::fromBeats(startBar + lengthInBars), tempoSequence);

  // Validate the audio file
  juce::File file(filePath);
  if (!file.existsAsFile()) {
    std::cerr << "Audio file does not exist: " << filePath << std::endl;
    return false;
  }

  // Assign the audio file to the clip
  te::AudioFile audioFile(edit->engine, file);

  if (audioFile.isValid())
    if (auto newClip = audioTrack->insertWaveClip(
            file.getFileNameWithoutExtension(),
            file,
            {{{}, te::TimeDuration::fromSeconds(audioFile.getLength())}, {}},
            false))
      return newClip;
  return true;
}

int TrackManager::addMidiClip(int trackID, double startBar, double lengthInBars) {
  if (!edit) {
    std::cout << "addMidiClip - Edit not found";
    return -1;
  }

  // Check if the track exists
  auto targetTrack = edit->getTrackList().at(trackID);
  if (!targetTrack) {
    std::cout << "No track found for id - " << trackID;
    return -1;
  }

  std::cout << "target track found";

  auto track = dynamic_cast<te::AudioTrack*>(targetTrack);
  if (!track)
    return -1;

  std::cout << "dynamic_cast to audio track done";

  // Convert bars to time using the TempoSequence
  auto& tempoSequence = edit->tempoSequence;
  te::TimePosition startPosition = te::toTime(te::BeatPosition::fromBeats(startBar), tempoSequence);
  te::TimePosition endPosition =
      te::toTime(te::BeatPosition::fromBeats(startBar + lengthInBars), tempoSequence);

  // Define time range for the MIDI clip
  te::TimeRange timeRange(startPosition, endPosition);

  // Insert a new MIDI clip
  auto clip = track->insertNewClip(te::TrackItem::Type::midi,
                                   "MIDI Clip - " + std::to_string(trackID),
                                   timeRange,
                                   nullptr);
  return clip->itemID.getRawID();
}

// bool TrackManager::loadInstrument(int trackID, const std::string &presetPath)
// {
//     auto it = tracks.find(trackID);
//     if (it == tracks.end())
//         return false;

//     auto audioTrack = dynamic_cast<te::AudioTrack *>(it->second.get());
//     if (!audioTrack)
//         return false;

//     try
//     {
//         // Load the instrument preset (assuming the sampler is set up
//         correctly) auto sampler =
//         audioTrack->pluginList.findFirstPluginOfType<te::SamplerPlugin>(); if
//         (sampler)
//         {
//             const auto error =
//             sampler->addSound(defaultSampleFiles[channelCount++].getFullPathName(),
//             channel->name.get(), 0.0, 0.0, 1.0f);
//             sampler->setSoundParams(sampler->getNumSounds() - 1,
//             channel->noteNumber, channel->noteNumber, channel->noteNumber);
//             jassert(error.isEmpty());
//             // sampler->loadSample(presetPath);
//         }
//         else
//         {
//             std::cerr << "No sampler plugin found on track" << std::endl;
//             return false;
//         }
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << "Error loading instrument: " << e.what() << std::endl;
//         return false;
//     }

//     return true;
// }
