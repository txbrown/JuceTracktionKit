#include "AudioEngine.h"
#include <cstdio>

class ProgressRunner : public juce::ThreadPoolJob
{
public:
  ProgressRunner(te::EditRenderJob *j, void (*onprogresschange)(float))
      : ThreadPoolJob("Render"), job(j), progressCallback(onprogresschange) {}

  JobStatus runJob() override
  {
    while (job->progress < 1)
    {
      if (progressCallback)
      {
        progressCallback(job->getCurrentTaskProgress());
      }
      job->runJob();
    }
    return jobHasFinished;
  }

private:
  te::EditRenderJob *job;
  void (*progressCallback)(float);
};

AudioEngine *AudioEngine::create(const std::string &name)
{
  return new AudioEngine(name);
}

AudioEngine::AudioEngine(const std::string &name)
{
  try
  {
    engine = std::make_unique<te::Engine>(name);
    std::cout << "Engine created." << std::endl;

    AudioEngineHelpers::createTempProject(*engine);

    std::cout << "Temp project created." << std::endl;

    edit = std::make_unique<te::Edit>(*engine, te::Edit::EditRole::forEditing);
    // edit = te::Edit::createSingleTrackEdit(*engine, te::Edit::EditRole::forEditing);
    std::cout << "Edit created." << std::endl;
    for (auto &midiIn : engine->getDeviceManager().getMidiInDevices())
    {
      midiIn->setEnabled(true);
      midiIn->setMonitorMode(te::InputDevice::MonitorMode::automatic);
    }

    // edit->playInStopEnabled = true;
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error during AudioEngine initialization: " << e.what() << std::endl;
    engine.reset();
    throw;
  }
}

AudioEngine::~AudioEngine() = default;

void AudioEngine::startPlayback()
{
  edit->getTransport().play(false);
}

void AudioEngine::stopPlayback()
{
  edit->getTransport().stop(false, false);
}
void AudioEngine::setTempo(double bpm)
{
  edit->tempoSequence.getTempos()[0]->setBpm(bpm);
}

double AudioEngine::getTempo() const
{
  return edit->tempoSequence.getTempos()[0]->bpm;
}

bool AudioEngine::isPlaying() const
{
  return edit->getTransport().isPlaying();
}

void AudioEngine::exportAudio(const std::string &filePath, void (*onprogresschange)(float))
{
  try
  {
    juce::File outputFile(filePath);

    // throw error if file exists
    if (outputFile.exists())
    {
      throw std::runtime_error("File already exists");
    }

    // Validate the output file
    if (!te::Renderer::checkTargetFile(*engine, outputFile))
    {
      std::cerr << "Export failed: Invalid target file" << std::endl;
      return;
    }

    // Create render parameters
    te::Renderer::Parameters renderParams(*edit);
    renderParams.destFile = outputFile;
    renderParams.audioFormat = engine->getAudioFileFormatManager().getDefaultFormat();
    renderParams.bitDepth = 24;
    renderParams.sampleRateForAudio = engine->getDeviceManager().getSampleRate();
    renderParams.blockSizeForAudio = engine->getDeviceManager().getBlockSize();

    renderParams.time = te::TimeRange(te::TimePosition::fromSeconds(0.0),
                                      te::TimePosition::fromSeconds(edit->getLength().inSeconds()));

    renderParams.usePlugins = true;
    renderParams.useMasterPlugins = true;
    renderParams.tracksToDo = toBitSet(getAllTracks(*edit));

    auto job = tracktion::EditRenderJob::getOrCreateRenderJob(edit->engine,
                                                              renderParams,
                                                              false,
                                                              false,
                                                              false);
    jassert(job != nullptr);

    ProgressRunner runner(static_cast<te::EditRenderJob *>(job.get()), onprogresschange);
    runner.runJob();
  }
  catch (const std::exception &e)
  {
    std::cerr << "Export failed: " << e.what() << std::endl;
  }
}

const bool AudioEngine::isClickTrackEnabled()
{
  return edit->clickTrackEnabled;
}

void AudioEngine::enableClickTrack()
{
  // edit->clickTrackEnabled = true;
}

void AudioEngine::disableClickTrack()
{
  edit->clickTrackEnabled = false;
}

void retainAudioEngine(AudioEngine *engine)
{
  assert(engine);
  ++engine->refCount;
}

void releaseAudioEngine(AudioEngine *engine)
{
  assert(engine);
  if (--engine->refCount == 0)
  {
    delete engine;
  }
}

te::Edit *AudioEngine::getEdit() const
{
  return edit.get();
}
