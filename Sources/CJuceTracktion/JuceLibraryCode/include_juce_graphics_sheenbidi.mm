#include <memory>
#include <Foundation/Foundation.h>

// Implement stubs for the SheenBidi functions that JUCE needs
extern "C" {
    void* SBAlgorithmCreate() { return nullptr; }
    void* SBAlgorithmCreateParagraph(void*, size_t, int) { return nullptr; }
    void SBAlgorithmRelease(void*) {}
    int SBCodepointGetGeneralCategory(uint32_t) { return 0; }
    int SBCodepointGetScript(uint32_t) { return 0; }
    size_t SBLineGetLength(void*) { return 0; }
    size_t SBLineGetOffset(void*) { return 0; }
    size_t SBLineGetRunCount(void*) { return 0; }
    void* SBLineGetRunsPtr(void*) { return nullptr; }
    void SBLineRelease(void*) {}
    void* SBParagraphCreateLine(void*, size_t, size_t) { return nullptr; }
    uint8_t SBParagraphGetBaseLevel(void*) { return 0; }
    size_t SBParagraphGetLength(void*) { return 0; }
    void* SBParagraphGetLevelsPtr(void*) { return nullptr; }
    size_t SBParagraphGetOffset(void*) { return 0; }
    void SBParagraphRelease(void*) {}
    void* SBParagraphRetain(void*) { return nullptr; }
} 