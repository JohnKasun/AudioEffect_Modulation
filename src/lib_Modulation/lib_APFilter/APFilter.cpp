#include "APFilter.h"

APFilter::APFilter() : mInputDelayLine(1), mOutputDelayLine(1) {
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
  auto prevInput = mInputDelayLine.getPostInc();
  auto prevOutput = mOutputDelayLine.getPostInc();
  auto output = mGain * input + prevInput - mGain * prevOutput;
  mInputDelayLine.putPostInc(input);
  mOutputDelayLine.putPostInc(output);
  return output;
}
