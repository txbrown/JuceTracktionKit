#include "./include/TracktionEngine.h"

void TracktionEngine::initialize()
{
    if (engine)
    {
        engine->getDeviceManager().initialise();
    }
}

void TracktionEngine::shutdown()
{
    if (engine)
    {
        engine->getDeviceManager().closeDevices();
    }
}

const char *TracktionEngine::getVersion()
{
    return engine->getVersion().toRawUTF8();
}
