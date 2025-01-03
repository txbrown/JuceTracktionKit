#pragma once

#include "EngineHelpers.h"
#include "swift/bridging"
#include <atomic>
#include <cassert>
#include <memory>
#include <string>
#include <tracktion_engine/tracktion_engine.h>

class AudioEngine {
public:
  static AudioEngine* create(const std::string& name);
  AudioEngine(const AudioEngine&) = delete;
  ~AudioEngine();

  void startPlayback() SWIFT_NAME(start());
  void stopPlayback() SWIFT_NAME(stop());
  void setTempo(double bpm) SWIFT_COMPUTED_PROPERTY;
  double getTempo() const SWIFT_COMPUTED_PROPERTY;
  bool isPlaying() const SWIFT_COMPUTED_PROPERTY;
  bool exportAudio(const std::string& filePath) SWIFT_NAME(exportAudio(to:));
  const bool isClickTrackEnabled();
  void enableClickTrack();
  void disableClickTrack();
  te::Edit* getEdit() const SWIFT_COMPUTED_PROPERTY;

private:
  AudioEngine(const std::string& name);

  std::unique_ptr<te::Engine> engine;
  std::unique_ptr<te::Edit> edit;
  te::TransportControl* transport;

  std::atomic<int> refCount{0};

  friend void retainAudioEngine(AudioEngine*);
  friend void releaseAudioEngine(AudioEngine*);
} SWIFT_IMMORTAL_REFERENCE;

void retainAudioEngine(AudioEngine*);
void releaseAudioEngine(AudioEngine*);
