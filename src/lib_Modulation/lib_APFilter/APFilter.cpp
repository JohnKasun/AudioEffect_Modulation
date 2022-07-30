#include "APFilter.h"

APFilter::APFilter(float sampleRate) :
	mSampleRate(sampleRate) { 
	mBreakFrequency = mSampleRate / 2.0f;
}

APFilter::~APFilter() {
}

Error_t APFilter::setBreakFrequency(float newBreakFrequency) {
  if (newBreakFrequency < -1 || newBreakFrequency > 1) return Error_t::kFunctionInvalidArgsError;

  //TODO: Calculate how long to output zeros based on breakfrequency and samplerate
  mBreakFrequency = newBreakFrequency;
  return Error_t::kNoError;
}

float APFilter::getBreakFrequency() const { return mBreakFrequency; }

float APFilter::process(float input) {
	//TODO: Implement some form of delay
  float a = (tanf(M_PI * mBreakFrequency / mSampleRate) - 1) / (tanf(M_PI * mBreakFrequency / mSampleRate) + 1);
  auto output = (a * input) - (a * mPrevOutput) + mPrevInput;
  mPrevInput = input;
  mPrevOutput = output;
  return output;
}
