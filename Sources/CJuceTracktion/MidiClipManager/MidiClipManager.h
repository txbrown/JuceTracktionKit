#pragma once

#include "../AudioEngine/EngineHelpers.h"
#include "swift/bridging"
#include <string>
#include <tracktion_engine/tracktion_engine.h>
#include <vector>

class MidiClipManager {
public:
  MidiClipManager(te::Edit* edit);
  ~MidiClipManager();

  int createMidiClip(int trackID, const std::string& name, double startBar, double lengthInBars)
      SWIFT_NAME(MidiClipManager.createMidiClip(trackID:name:startBar:lengthInBars:));
  bool deleteMidiClip(int trackID, int clipID)
      SWIFT_NAME(MidiClipManager.deleteMidiClip(trackID:clipID:));
  bool addNote(int clipID, int noteNumber, double startTime, double length, int velocity)
      SWIFT_NAME(MidiClipManager.addNote(clipID:noteNumber:startTime:length:velocity:));
  bool removeNote(int clipID, int noteNumber, double startTime)
      SWIFT_NAME(MidiClipManager.removeNote(clipID:noteNumber:startTime:));
  std::vector<std::string> listNotes(int clipID) SWIFT_NAME(MidiClipManager.listNotes(clipID:));
  void createSamplerPlugin(int trackID, int clipID, std::vector<std::string> defaultSampleFiles)
      SWIFT_NAME(MidiClipManager.createSamplerPlugin(trackID:clipID:defaultSampleFiles:));

private:
  te::SafeSelectable<te::Edit> edit;
  te::MidiClip* getMidiClipByID(int clipID);
};