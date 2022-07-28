#pragma once

#include "RingBuffer.h"
#include "ErrorDef.h"

class APFilter {
 public:
  APFilter();
  ~APFilter();

  Error_t setGain(float newGain);
  float getGain() const;
  float process(float input);

 private:
  CRingBuffer<float> mInputDelayLine;
  CRingBuffer<float> mOutputDelayLine;
  float mPrevInput = 0.0f;
  float mPrevOutput = 0.0f;
  float mGain = 1.0f;
};