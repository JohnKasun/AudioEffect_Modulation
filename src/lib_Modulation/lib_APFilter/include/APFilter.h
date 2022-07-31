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
  int getLatency() const;
  float process(float input);

 private:
  float mParamRanges[numParams][2]{};
  float mParamValues[numParams]{};
  float mPrevInput = 0.0f;
  float mPrevOutput = 0.0f;
  float mSampleRate = 1.0f;
  int mInitialDelay = 0.0f;

  bool isParamInRange(Param param, float value);
};