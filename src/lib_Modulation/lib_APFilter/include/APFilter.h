#pragma once

#include "RingBuffer.h"
#include "ErrorDef.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <memory>

class APFilter {
 public:
  APFilter(float sampleRate);
  ~APFilter();

  Error_t setBreakFrequency(float value);
  float getBreakFrequency() const;
  float process(float input);

 private:
  int mInitialDelay;
  float mPrevInput = 0.0f;
  float mPrevOutput = 0.0f;
  float mSampleRate = 1.0f;
  float mBreakFrequency;
};