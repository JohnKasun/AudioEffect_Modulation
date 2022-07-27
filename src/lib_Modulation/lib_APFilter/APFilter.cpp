#include "APFilter.h"

APFilter::APFilter() {
}

APFilter::~APFilter() {
}

void APFilter::setGain(float newGain) {
  mGain = newGain;
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
