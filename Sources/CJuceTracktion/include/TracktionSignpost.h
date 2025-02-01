#pragma once

#ifdef __APPLE__
#include <os/signpost.h>
#include <os/log.h>

#define TRACKTION_SIGNPOST_ENABLED 1

#define TRACKTION_SIGNPOST_LOG(name) \
    static os_log_t log = os_log_create("com.tracktion", name)

#define TRACKTION_SIGNPOST_BEGIN(log, name) \
    os_signpost_interval_begin(log, OS_SIGNPOST_ID_EXCLUSIVE, name)

#define TRACKTION_SIGNPOST_END(log, name) \
    os_signpost_interval_end(log, OS_SIGNPOST_ID_EXCLUSIVE, name)

#define TRACKTION_SIGNPOST_EVENT(log, name) \
    os_signpost_event_emit(log, OS_SIGNPOST_ID_EXCLUSIVE, name)

#else

#define TRACKTION_SIGNPOST_ENABLED 0
#define TRACKTION_SIGNPOST_LOG(name)
#define TRACKTION_SIGNPOST_BEGIN(log, name)
#define TRACKTION_SIGNPOST_END(log, name)
#define TRACKTION_SIGNPOST_EVENT(log, name)

#endif