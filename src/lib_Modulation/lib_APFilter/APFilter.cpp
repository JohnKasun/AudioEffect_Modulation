#include "APFilter.h"

APFilter::APFilter() : mInputDelayLine(1), mOutputDelayLine(1) {}

APFilter::~APFilter() {
}

Error_t APFilter::setGain(float newGain) {
  if (newGain < -1 || newGain > 1) return Error_t::kFunctionInvalidArgsError;

  mGain = newGain;
  return Error_t::kNoError;
}

float APFilter::getGain() const {
  return mGain;
}

float APFilter::process(float input) {
  auto output = (mGain * input) - (mGain * mPrevOutput) + mPrevInput;
  mPrevInput = input;
  mPrevOutput = output;
  return output;
}
