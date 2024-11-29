#include "AudioEngine.h"

namespace EngineHelpers
{

    std::atomic<bool> shouldStop{false};

    inline te::Project::Ptr createTempProject(te::Engine &engine)
    {
        auto file = engine.getTemporaryFileManager().getTempDirectory().getChildFile("temp_project").withFileExtension(te::projectFileSuffix);
        te::ProjectManager::TempProject tempProject(engine.getProjectManager(), file, true);
        return tempProject.project;
    }

    // Custom message loop for processing events
    inline void startMessageManagerLoop()
    {
        shouldStop = false;

        // Launch a background thread
        std::thread([]
                    {
        while (!shouldStop)
        {
            juce::MessageManager::getInstance()->runDispatchLoop(); // 10 ms
        } })
            .detach();
    }
}

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

        EngineHelpers::createTempProject(*engine);

        std::cout << "Temp project created." << std::endl;

        edit = te::Edit::createSingleTrackEdit(*engine, te::Edit::EditRole::forEditing);
        std::cout << "Edit created." << std::endl;
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

bool AudioEngine::exportAudio(const std::string &filePath)
{
    // Basic export implementation
    return true;
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
