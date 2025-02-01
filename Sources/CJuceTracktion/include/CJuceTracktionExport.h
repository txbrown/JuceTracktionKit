#pragma once

#if defined(_WIN32) || defined(__CYGWIN__)
#ifdef CJUCETRACKTION_EXPORTS
#define CJUCETRACKTION_API __declspec(dllexport)
#else
#define CJUCETRACKTION_API __declspec(dllimport)
#endif
#else
#if __GNUC__ >= 4
#define CJUCETRACKTION_API __attribute__((visibility("default")))
#else
#define CJUCETRACKTION_API
#endif
#endif