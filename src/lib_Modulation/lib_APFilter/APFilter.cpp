#include "APFilter.h"

APFilter::APFilter(float sampleRate) :
	mSampleRate(sampleRate), mInitialDelay(17) { 
  mBreakFrequency = mSampleRate / 8.0f;
}

APFilter::~APFilter() {
}

Error_t APFilter::setBreakFrequency(float newBreakFrequency) {
  if (newBreakFrequency < mSampleRate / 16.0f || newBreakFrequency > mSampleRate / 4.0f) return Error_t::kFunctionInvalidArgsError;

  mBreakFrequency = newBreakFrequency;
  return Error_t::kNoError;
}

float APFilter::getBreakFrequency() const { return mBreakFrequency; }

float APFilter::process(float input) {
  float a = (tanf(M_PI * mBreakFrequency / mSampleRate) - 1) / (tanf(M_PI * mBreakFrequency / mSampleRate) + 1);
  auto output = (a * input) - (a * mPrevOutput) + mPrevInput;
  mPrevInput = input;
  mPrevOutput = output;
  if (mInitialDelay > 0) {
    mInitialDelay--;
    return 0.0f;
  }
  return output;
}
