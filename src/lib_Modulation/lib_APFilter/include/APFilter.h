#pragma once

#include "RingBuffer.h"
#include "ErrorDef.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <memory>

class APFilter {
 public:
  enum Param {
    BreakFreq,

    numParams
  };
  APFilter(float sampleRate);
  ~APFilter();

  Error_t setParam(Param param, float value);
  float getParam(Param param) const;
  float process(float input);

  float getMaxBreakFrequency() const;
  float getMinBreakFrequency() const;
  int getLatency() const;

 private:
  float mParamRanges[numParams][2]{};
  float mParamValues[numParams]{};
  float mPrevInput = 0.0f;
  float mPrevOutput = 0.0f;
  float mSampleRate = 1.0f;
  int mLatency = 0;
  int mLatencyCounter = 0;

  bool isParamInRange(Param param, float value);
};