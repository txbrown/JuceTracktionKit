#pragma once

#include "../AudioEngine/EngineHelpers.h"
#include "../MidiNote/MidiNote.h"
#include "swift/bridging"
#include <string>
#include <tracktion_engine/tracktion_engine.h>
#include <vector>

class MidiClipManager
{
public:
  MidiClipManager(te::Edit *edit);
  ~MidiClipManager();
  static MidiClipManager *create(te::Edit *edit);

  int createMidiClip(int trackID, const std::string &name, double startBar, double lengthInBars)
      SWIFT_NAME(MidiClipManager.createMidiClip(trackID:name:startBar:lengthInBars:));
  bool deleteMidiClip(int trackID, int clipID)
      SWIFT_NAME(MidiClipManager.deleteMidiClip(trackID:clipID:));
  bool addNote(int clipID, const MidiNote &note) SWIFT_NAME(MidiClipManager.addNote(clipID:note:));
  bool removeNote(int clipID, int noteNumber, double startTime)
      SWIFT_NAME(MidiClipManager.removeNote(clipID:noteNumber:startTime:));
  std::vector<MidiNote> getNotes(int clipID) SWIFT_NAME(MidiClipManager.getNotes(clipID:));

private:
  std::unique_ptr<te::Edit> edit;
  te::MidiClip *getMidiClipByID(int clipID);
  std::atomic<int> refCount{0};

  friend void retainMidiClipManager(MidiClipManager *);
  friend void releaseMidiClipManager(MidiClipManager *);
} SWIFT_IMMORTAL_REFERENCE;

void retainMidiClipManager(MidiClipManager *);
void releaseMidiClipManager(MidiClipManager *);