#include "Chorus.h"

Chorus::Chorus()
{
	mParamRanges[static_cast<int>(RangedParameter::Depth)][0] = 0;
	mParamRanges[static_cast<int>(RangedParameter::Depth)][1] = 0.6;
	mParamRanges[static_cast<int>(RangedParameter::Speed)][0] = 0;
	mParamRanges[static_cast<int>(RangedParameter::Speed)][1] = 5;
}

Chorus::~Chorus()
{
	reset();
}

Error_t Chorus::init(float sampleRate)
{
	if (sampleRate <= 0.0f)
		return Error_t::kFunctionInvalidArgsError;

	reset();

	mSampleRate = sampleRate;
	mDelayLine.reset(new CRingBuffer<float>(mSampleRate * mParamRanges[static_cast<int>(RangedParameter::Depth)][1] * 2));
	mDelayLine->setWriteIdx(mSampleRate * mParamRanges[static_cast<int>(RangedParameter::Depth)][1]);
	for (int i = 0; i < mNumVoices; i++) {
		auto radianOffset = double{ 2.0f * M_PI / mNumVoices * i };
		mLfos.emplace_back(new Lfo(mSampleRate));
		mLfos.back()->setParam(Lfo::Param_t::phaseInRadians, static_cast<float>(radianOffset));
	}
	mIsInitialized = true;
	return Error_t::kNoError;
}

Error_t Chorus::reset()
{
	if (mIsInitialized) {
		mDelayLine.reset();
		mLfos.clear();
		mSampleRate = 1.0f;
		mIsInitialized = false;
	}
	return Error_t::kNoError;
}

Error_t Chorus::setDepth(float newDepth)
{
	if (!isParamInRange(RangedParameter::Depth, newDepth))
		return Error_t::kFunctionInvalidArgsError;

	for (auto& lfo : mLfos) {
		lfo->setParam(Lfo::Param_t::amplitude, newDepth * mSampleRate);
	}
	return Error_t::kNoError;
}

Error_t Chorus::setSpeed(float newSpeed)
{
	if (!isParamInRange(RangedParameter::Speed, newSpeed))
		return Error_t::kFunctionInvalidArgsError;

	for (auto& lfo : mLfos) {
		lfo->setParam(Lfo::Param_t::freqInHz, newSpeed);
	}
	return Error_t::kNoError;
}

Error_t Chorus::setShape(Chorus::Shape newShape)
{
	return Error_t();
}

float Chorus::getDepth() const
{
	return mLfos[0]->getParam(Lfo::Param_t::amplitude);
}

float Chorus::getSpeed() const
{
	return mLfos[0]->getParam(Lfo::Param_t::freqInHz);
}

Chorus::Shape Chorus::getShape() const
{
	return Chorus::Shape();
}


Error_t Chorus::process(const float const* inputBuffer, float* outputBuffer, const int numSamples)
{
	for (int i = 0; i < numSamples; i++) {
		mDelayLine->putPostInc(inputBuffer[i]);
		auto sum = float{ 0 };
		for (auto& lfo : mLfos) {
			sum += mDelayLine->get(lfo->process());
		}
		outputBuffer[i] = sum;
		mDelayLine->getPostInc();
	}
	return Error_t::kNoError;
}

bool Chorus::isParamInRange(Chorus::RangedParameter param, float value) const
{
	return (mParamRanges[static_cast<int>(param)][0] <= value && value <= mParamRanges[static_cast<int>(param)][1]);
}
