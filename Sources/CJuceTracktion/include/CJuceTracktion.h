#pragma once

// Umbrella header that includes all public headers
#include "../AudioEngine/AudioEngine.h"
#include <juce_events/juce_events.h>
#include <thread>
#include <atomic>

extern "C"
{
    void startMessageManagerLoop();
    void runCustomMessageLoop(int milliseconds);
    void stopMessageManagerLoop();
}

namespace
{
    std::atomic<bool> shouldStop{false};
    std::thread messageManagerThread;
}

// Custom message loop for processing events
void runCustomMessageLoop(int milliseconds)
{
    auto *messageManager = juce::MessageManager::getInstance();
    const auto startTime = juce::Time::getMillisecondCounter();

    while (juce::Time::getMillisecondCounter() - startTime < milliseconds)
    {
        // Check for pending messages and process them
        // if (!messageManager->().isEmpty())
        // break; // Exit if no more messages
    }
}

// Start the event loop in a background thread
void startMessageManagerLoop()
{
    shouldStop = false;

    // Ensure only one thread is running
    if (messageManagerThread.joinable())
        return;

    messageManagerThread = std::thread([]
                                       {
        auto* messageManager = juce::MessageManager::getInstance();

        while (!shouldStop)
        {
            // if (!messageManager->dispatchNextMessage())
                std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Avoid tight loops
        } });
}

// Stop the background event loop
void stopMessageManagerLoop()
{
    shouldStop = true;

    if (messageManagerThread.joinable())
        messageManagerThread.join();
}
