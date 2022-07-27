#pragma once

#include "RingBuffer.h"

class APFilter {
 public:
  APFilter();
  ~APFilter();

  void setGain(float newGain);
  float getGain() const;
  float process(float input);

 private:
  float mPrevInput = 0.0f;
  float mPrevOutput = 0.0f;
  float mGain = 1.0f;
};