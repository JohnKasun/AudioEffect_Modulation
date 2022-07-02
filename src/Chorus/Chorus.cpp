#include "Chorus.h"

Chorus::Chorus()
{
}

Chorus::~Chorus()
{
}

Error_t Chorus::init(float sampleRate)
{
	if (sampleRate <= 0.0f)
		return Error_t::kFunctionInvalidArgsError;

	reset();

	mSampleRate = sampleRate;
	mDelayLine.reset(new CRingBuffer<float>(mSampleRate * mMaxDelayInSec));
	mIsInitialized = true;
}

Error_t Chorus::reset()
{
	if (mIsInitialized) {
		mDelayLine.reset();
		mLfos.clear();
		mSampleRate = 1.0f;
		mIsInitialized = false;
	}
}


Error_t Chorus::process(const float const* inputBuffer, float const* outputBuffer, const int numSamples)
{
	return Error_t();
}

bool Chorus::isParamInRange(Chorus::Parameter param, auto value) const
{
	return false;
}
