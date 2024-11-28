#pragma once
#include "swift/bridging"
#include "../extra/JuceTracktion/JuceLibraryCode/JuceHeader.h"

namespace te = tracktion;

class EditTimeRange
{
public:
    EditTimeRange(double start, double end);
    double start() const;
    double end() const;
    const te::EditTimeRange &getRange() const;

private:
    te::EditTimeRange range;
};