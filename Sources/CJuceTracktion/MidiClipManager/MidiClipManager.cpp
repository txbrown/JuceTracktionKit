#include "MidiClipManager.h"
#include <cassert>
#include <iostream>
#include <sys/wait.h>

static juce::String organPatch =
    "<PLUGIN type=\"4osc\" windowLocked=\"1\" id=\"1069\" enabled=\"1\" filterType=\"1\" "
    "presetDirty=\"0\" presetName=\"4OSC: Organ\" filterFreq=\"127.00000000000000000000\" "
    "ampAttack=\"0.60000002384185791016\" ampDecay=\"10.00000000000000000000\" "
    "ampSustain=\"100.00000000000000000000\" ampRelease=\"0.40000000596046447754\" "
    "waveShape1=\"4\" tune2=\"-24.00000000000000000000\" waveShape2=\"4\"> <MACROPARAMETERS "
    "id=\"1069\"/> <MODIFIERASSIGNMENTS/> <MODMATRIX/> </PLUGIN>";

static juce::String leadPatch =
    "<PLUGIN type=\"4osc\" windowLocked=\"1\" id=\"1069\" enabled=\"1\" "
    "filterType=\"1\" waveShape1=\"3\" filterFreq=\"100\"><MACROPARAMETERS "
    "id=\"1069\"/><MODIFIERASSIGNMENTS/><MODMATRIX/></PLUGIN>";

MidiClipManager::MidiClipManager(te::Edit* edit) : edit(edit) {}

MidiClipManager::~MidiClipManager() = default;

int MidiClipManager::createMidiClip(int trackID,
                                    const std::string& name,
                                    double startBar,
                                    double lengthInBars) {
  if (!edit)
    return -1;

  // Find the track by ID
  te::Track* targetTrack = edit->trackCache.findItem(te::EditItemID::fromRawID(trackID));

  auto* audioTrack = dynamic_cast<te::AudioTrack*>(targetTrack);
  if (!audioTrack)
    return -1;

  // Convert bar positions to time positions
  te::TimePosition startTime =
      te::toTime(te::BeatPosition::fromBeats(startBar), edit->tempoSequence);
  te::TimePosition endTime =
      te::toTime(te::BeatPosition::fromBeats(startBar + lengthInBars), edit->tempoSequence);

  // Create and insert the MIDI clip
  te::TimeRange timeRange(startTime, endTime);
  auto* clip = dynamic_cast<te::MidiClip*>(
      audioTrack->insertNewClip(te::TrackItem::Type::midi, name, timeRange, nullptr));

  if (!clip)
    return -1;

  if (auto midiClip = clip) {
    AudioEngineHelpers::loopAroundClip(*midiClip);
  }
  // end temp

  std::cout << "new clip created - " << clip->itemID.getRawID();
  return clip->itemID.getRawID();
}

bool MidiClipManager::deleteMidiClip(int trackID, int clipID) {
  if (!edit)
    return false;

  auto clip = edit->clipCache.findItem(te::EditItemID::fromRawID(clipID));
  if (!clip)
    return false;

  auto targetTrack = edit->trackCache.findItem(te::EditItemID::fromRawID(trackID));
  auto* audioTrack = dynamic_cast<te::AudioTrack*>(targetTrack);
  if (!audioTrack)
    return false;
  clip->removeFromParent();
  return true;
}

bool MidiClipManager::addNote(int clipID, const MidiNote& note) {
  auto clip =
      dynamic_cast<te::MidiClip*>(edit->clipCache.findItem(te::EditItemID::fromRawID(clipID)));
  if (!clip)
    return false;

  auto& midiList = clip->getSequence();
  auto startBeat = te::BeatPosition::fromBeats(note.startBeat);
  auto lengthBeats = te::BeatDuration::fromBeats(note.lengthInBeats);
  auto um = &edit.get()->getUndoManager();

  midiList.addNote(note.noteNumber, startBeat, lengthBeats, note.velocity, 0, um);
  return true;
}

bool MidiClipManager::removeNote(int clipID, int noteNumber, double startTime) {
  auto clip =
      dynamic_cast<te::MidiClip*>(edit->clipCache.findItem(te::EditItemID::fromRawID(clipID)));
  if (!clip)
    return false;

  auto& midiList = clip->getSequence();
  auto um = &edit.get()->getUndoManager();

  auto startBeat = te::BeatPosition::fromBeats(startTime);

  for (auto* note : midiList.getNotes()) {
    if (note->getNoteNumber() == noteNumber && note->getStartBeat() == startBeat) {
      midiList.removeNote(*note, um);
      return true;
    }
  }

  return false;
}

std::vector<MidiNote> MidiClipManager::getNotes(int clipID) {
  std::vector<MidiNote> notesList;

  auto clip =
      dynamic_cast<te::MidiClip*>(edit->clipCache.findItem(te::EditItemID::fromRawID(clipID)));
  if (!clip)
    return notesList;

  auto& midiList = clip->getSequence();
  for (const auto* note : midiList.getNotes()) {
    notesList.push_back(MidiNoteUtils::fromTracktionNote(*note));
  }

  return notesList;
}

void create4OSCPlugin(te::AudioTrack* track, te::Edit* edit) {
  //==============================================================================
  if (auto synth = dynamic_cast<te::FourOscPlugin*>(
          edit->getPluginCache().createNewPlugin(te::FourOscPlugin::xmlTypeName, {}).get())) {
    if (auto e = parseXML(organPatch)) {
      auto vt = juce::ValueTree::fromXml(*e);

      if (vt.isValid())
        synth->restorePluginStateFromValueTree(vt);
    }

    track->pluginList.insertPlugin(*synth, 0, nullptr);
  }
}

void MidiClipManager::createSamplerPlugin(int trackID,
                                          int clipID,
                                          std::vector<std::string> defaultSampleFiles) {
  std::cout << "Creating sampler plugin - " << trackID;
  if (!edit)
    assert("Edit not found!");

  auto targetTrack = edit->trackCache.findItem(te::EditItemID::fromRawID(trackID));
  if (!targetTrack)
    assert("No track found for given id");

  auto* audioTrack = dynamic_cast<te::AudioTrack*>(targetTrack);
  if (!audioTrack) {
    assert("No track found for given id");
    std::cout << "No track found for given id";
    return;
  }

  std::cout << "Let's create the sampler plugin";
  if (auto stepClip = dynamic_cast<te::MidiClip*>(
          audioTrack->findClipForID(te::EditItemID::fromRawID(clipID)))) {
    if (auto sampler = dynamic_cast<te::SamplerPlugin*>(
            edit->getPluginCache().createNewPlugin(te::SamplerPlugin::xmlTypeName, {}).get())) {
      std::cout << "insertPlugin";
      stepClip->getTrack()->pluginList.insertPlugin(*sampler, 0, nullptr);

      int channelCount = 0;

      int noteNumber = 36;
      for (auto sample : defaultSampleFiles) {
        const auto error = sampler->addSound(sample, sample, 0.0, 0.0, 1.0f);
        sampler->setSoundParams(sampler->getNumSounds() - 1, noteNumber, noteNumber, noteNumber);
        noteNumber++;
        jassert(error.isEmpty());
      }
    }
  } else {
    std::cout << "Step clip not created yet";
    jassertfalse;  // StepClip not been created yet?
  }
}

namespace {
static void loadFileIntoSamplerChannel(te::StepClip& clip, int channelIndex, const juce::File& f) {
  // Find SamplerPlugin for the Clip's Track
  if (auto sampler = clip.getTrack()->pluginList.findFirstPluginOfType<te::SamplerPlugin>()) {
    // Update the Sound layer source
    sampler->setSoundMedia(channelIndex, f.getFullPathName());

    // Then update the channel name
    clip.getChannels()[channelIndex]->name = f.getFileNameWithoutExtension();
  } else {
    jassertfalse;  // No SamplerPlugin added yet?
  }
}
}  // namespace
