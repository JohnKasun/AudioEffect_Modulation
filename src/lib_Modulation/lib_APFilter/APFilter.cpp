#include "APFilter.h"

APFilter::APFilter(float sampleRate) :
	mSampleRate(sampleRate){ 
  mParamRanges[BreakFreq][0] = mSampleRate / 16.0f;
  mParamRanges[BreakFreq][1] = mSampleRate / 4.0f;
  mLatency = static_cast<int>(mSampleRate / mParamRanges[BreakFreq][0]);

  mParamValues[BreakFreq] = mSampleRate / 8.0f;
}

APFilter::~APFilter() {
}

Error_t APFilter::setParam(Param param, float value) {
  if (!isParamInRange(param, value))
    return Error_t::kFunctionInvalidArgsError;

  mParamValues[param] = value;
  return Error_t::kNoError;
}

float APFilter::getParam(Param param) const { return mParamValues[param]; }

int APFilter::getLatency() const { return mLatency; }

float APFilter::process(float input) {
  float a = (tanf(M_PI * mParamValues[BreakFreq] / mSampleRate) - 1) / (tanf(M_PI * mParamValues[BreakFreq] / mSampleRate) + 1);
  auto output = (a * input) - (a * mPrevOutput) + mPrevInput;
  mPrevInput = input;
  mPrevOutput = output;
  if (mLatencyCounter < mLatency) {
    mLatencyCounter++;
    return 0.0f;
  }
  return output;
}

bool APFilter::isParamInRange(Param param, float value) { return (value >= mParamRanges[param][0] && value <= mParamRanges[param][1]); }
