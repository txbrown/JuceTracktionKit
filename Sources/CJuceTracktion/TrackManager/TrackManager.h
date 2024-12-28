#pragma once

#include "../AudioEngine/EngineHelpers.h"
#include <juce_core/juce_core.h>
#include <map>
#include <string>
#include <swift/bridging>
#include <tracktion_engine/tracktion_engine.h>

class TrackManager {
public:
  TrackManager(te::Edit* edit);
  ~TrackManager();

  int createAudioTrack(const std::string& name) SWIFT_NAME(TrackManager.createAudioTrack(name:));
  bool removeTrack(int trackID) SWIFT_NAME(TrackManager.removeTrack(byID:));
  bool addAudioClip(int trackID, const std::string& filePath, double startBar, double lengthInBars)
      SWIFT_NAME(TrackManager.addAudioClip(forTrackID:filePath:startBar:lengthInBars:));
  int addMidiClip(int trackID, double startBar, double lengthInBars)
      SWIFT_NAME(TrackManager.addMidiClip(forTrackID:startBar:lengthInBars:));
  void createSamplerPlugin(int trackID, std::vector<std::string> defaultSampleFiles)
      SWIFT_NAME(TrackManager.createSamplerPlugin(trackID:defaultSampleFiles:));

private:
  te::Edit* edit;
  std::map<int, juce::ReferenceCountedObjectPtr<te::Track>> tracks;
  int nextTrackID = 0;
};
