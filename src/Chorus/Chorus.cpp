#include "Chorus.h"

Chorus::Chorus()
{
	mParamRanges[static_cast<int>(RangedParameter::Delay)][0] = 0;
	mParamRanges[static_cast<int>(RangedParameter::Delay)][1] = 20;
	mParamRanges[static_cast<int>(RangedParameter::Depth)][0] = 0;
	mParamRanges[static_cast<int>(RangedParameter::Depth)][1] = 50;
	mParamRanges[static_cast<int>(RangedParameter::Speed)][0] = 0;
	mParamRanges[static_cast<int>(RangedParameter::Speed)][1] = 1;
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
	auto maxDelay = int{ CUtil::float2int<int>(mSampleRate * mParamRanges[static_cast<int>(RangedParameter::Delay)][1] / 1000.0f) };
	auto maxDepth = int{ CUtil::float2int<int>(mSampleRate * mParamRanges[static_cast<int>(RangedParameter::Depth)][1] / 1000.0f) };
	mDelayLine.reset(new CRingBuffer<float>(1 + maxDelay + maxDepth));
	mLfo.reset(new Lfo(mSampleRate));
	mLfo->setParam(Lfo::Param_t::phaseInRadians, M_PI / 2.0f);
	mIsInitialized = true;
	return Error_t::kNoError;
}

Error_t Chorus::reset()
{
	if (mIsInitialized) {
		mDelayLine.reset();
		mLfo.reset();
		mSampleRate = 1.0f;
		mIsInitialized = false;
	}
	return Error_t::kNoError;
}

Error_t Chorus::setDelay(float newDelayInMs)
{
	if (!isParamInRange(RangedParameter::Delay, newDelayInMs))
		return Error_t::kFunctionInvalidArgsError;

	auto newDelayInSamp = float{ newDelayInMs * mSampleRate / 1000.0f};
	auto newDc = float{ newDelayInSamp + mLfo->getParam(Lfo::Param_t::amplitude) };
	mLfo->setParam(Lfo::Param_t::dc, -1.0f * newDc);
	return Error_t::kNoError;
}

Error_t Chorus::setDepth(float newDepthInMs)
{
	if (!isParamInRange(RangedParameter::Depth, newDepthInMs))
		return Error_t::kFunctionInvalidArgsError;

	auto newDepthInSamp = float{ newDepthInMs * mSampleRate / 1000.0f };
	auto newAmplitude = float{ newDepthInSamp / 2.0f };
	updateLfoDc(newAmplitude - mLfo->getParam(Lfo::amplitude));
	mLfo->setParam(Lfo::Param_t::amplitude, newAmplitude);
	return Error_t::kNoError;
}

Error_t Chorus::setSpeed(float newSpeedInHz)
{
	if (!isParamInRange(RangedParameter::Speed, newSpeedInHz))
		return Error_t::kFunctionInvalidArgsError;

	mLfo->setParam(Lfo::Param_t::freqInHz, newSpeedInHz);
	return Error_t::kNoError;
}

Error_t Chorus::setShape(Chorus::Shape newShape)
{
	return Error_t();
}

float Chorus::getDelay() const
{
	return ((-1.0f * mLfo->getParam(Lfo::Param_t::dc)) - mLfo->getParam(Lfo::Param_t::amplitude)) / mSampleRate * 1000.0f;
}

float Chorus::getDepth() const
{
	return mLfo->getParam(Lfo::Param_t::amplitude) * 2.0f / mSampleRate * 1000.0f;
}

float Chorus::getSpeed() const
{
	return mLfo->getParam(Lfo::Param_t::freqInHz);
}

Chorus::Shape Chorus::getShape() const
{
	return Chorus::Shape();
}


Error_t Chorus::process(const float const* inputBuffer, float* outputBuffer, const int numSamples)
{
	for (int i = 0; i < numSamples; i++) {
		mDelayLine->putPostInc(inputBuffer[i]);
		auto offset = float{ mLfo->process() };
		outputBuffer[i] = mDelayLine->get() + mDelayLine->get(offset);
		mDelayLine->getPostInc();
	}
	return Error_t::kNoError;
}

bool Chorus::isParamInRange(Chorus::RangedParameter param, float value) const
{
	return (mParamRanges[static_cast<int>(param)][0] <= value && value <= mParamRanges[static_cast<int>(param)][1]);
}

void Chorus::updateLfoDc(float dAmp)
{
	auto oldDc = float{ -1.0f * mLfo->getParam(Lfo::dc) };
	auto newDc = float{ oldDc + dAmp };
	mLfo->setParam(Lfo::dc, -1.0f * newDc);
}
