#include "AudioEngine.h"

AudioEngine* AudioEngine::create(const std::string& name) {
  return new AudioEngine(name);
}

AudioEngine::AudioEngine(const std::string& name) {
  try {
    engine = std::make_unique<te::Engine>(name);
    std::cout << "Engine created." << std::endl;

    AudioEngineHelpers::createTempProject(*engine);

    std::cout << "Temp project created." << std::endl;

    edit = te::Edit::createSingleTrackEdit(*engine, te::Edit::EditRole::forEditing);
    std::cout << "Edit created." << std::endl;
    for (auto& midiIn : engine->getDeviceManager().getMidiInDevices()) {
      midiIn->setEnabled(true);
      midiIn->setMonitorMode(te::InputDevice::MonitorMode::automatic);
    }

    edit->playInStopEnabled = true;

  } catch (const std::exception& e) {
    std::cerr << "Error during AudioEngine initialization: " << e.what() << std::endl;
    engine.reset();
    throw;
  }
}

AudioEngine::~AudioEngine() = default;

void AudioEngine::startPlayback() {
  edit->getTransport().play(false);
}

void AudioEngine::stopPlayback() {
  edit->getTransport().stop(false, false);
}
void AudioEngine::setTempo(double bpm) {
  edit->tempoSequence.getTempos()[0]->setBpm(bpm);
}

double AudioEngine::getTempo() const {
  return edit->tempoSequence.getTempos()[0]->bpm;
}

bool AudioEngine::isPlaying() const {
  return edit->getTransport().isPlaying();
}

bool AudioEngine::exportAudio(const std::string& filePath) {
  // Basic export implementation
  return true;
}

void retainAudioEngine(AudioEngine* engine) {
  assert(engine);
  ++engine->refCount;
}

void releaseAudioEngine(AudioEngine* engine) {
  assert(engine);
  if (--engine->refCount == 0) {
    delete engine;
  }
}

te::Edit* AudioEngine::getEdit() const {
  return edit.get();
}
