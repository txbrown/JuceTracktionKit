#pragma once
#include "swift/bridging"
#include "../extra/JuceTracktion/JuceLibraryCode/JuceHeader.h"
#include <atomic>
#include "EditTimeRange.h"

EditTimeRange::EditTimeRange(double s, double e)
    : range(te::engine::EditTimeRange(
          te::engine::EditTimeRange(
              te::TimePosition::fromSeconds(s),
              te::TimePosition::fromSeconds(e))))
{
}

double EditTimeRange::start() const
{
    return 0;
}

double EditTimeRange::end() const
{
    return 0;
}

const te::engine::EditTimeRange &EditTimeRange::getRange() const
{
    return range;
}