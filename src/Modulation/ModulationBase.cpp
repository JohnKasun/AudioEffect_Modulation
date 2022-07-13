#include "ModulationBase.h"

ModulationBase::ModulationBase(float sampleRate, float maxDelayInMs, float maxDepthInMs)
{
	mSampleRate = sampleRate;
	auto maxDelay = int{ CUtil::float2int<int>(mSampleRate * maxDelayInMs / 1000.0f) };
	auto maxDepth = int{ CUtil::float2int<int>(mSampleRate * maxDepthInMs / 1000.0f) };
	mDelayLine.reset(new CRingBuffer<float>(1 + maxDelay + maxDepth));
	mLfo.reset(new Lfo(mSampleRate));
	mLfo->setParam(Lfo::Param_t::phaseInRadians, M_PI / 2.0f);
}

ModulationBase::~ModulationBase()
{
	mDelayLine.reset();
	mLfo.reset();
	mSampleRate = 1.0f;
}

void ModulationBase::setDelay(float newDelayInMs)
{
	auto newDelayInSamp = float{ newDelayInMs * mSampleRate / 1000.0f };
	auto newDc = float{ newDelayInSamp + mLfo->getParam(Lfo::Param_t::amplitude) };
	mLfo->setParam(Lfo::Param_t::dc, -1.0f * newDc);
}

void ModulationBase::setDepth(float newDepthInMs)
{
	auto newDepthInSamp = float{ newDepthInMs * mSampleRate / 1000.0f };
	auto newAmplitude = float{ newDepthInSamp / 2.0f };
	updateLfoDc(newAmplitude - mLfo->getParam(Lfo::amplitude));
	mLfo->setParam(Lfo::Param_t::amplitude, newAmplitude);
}

void ModulationBase::setSpeed(float newSpeedInHz)
{
	mLfo->setParam(Lfo::Param_t::freqInHz, newSpeedInHz);
}

void ModulationBase::setShape(ModulationIf::Shape newShape)
{
	return void();
}

float ModulationBase::getDelay() const
{
	return ((-1.0f * mLfo->getParam(Lfo::Param_t::dc)) - mLfo->getParam(Lfo::Param_t::amplitude)) / mSampleRate * 1000.0f;
}

float ModulationBase::getDepth() const
{
	return mLfo->getParam(Lfo::Param_t::amplitude) * 2.0f / mSampleRate * 1000.0f;
}

float ModulationBase::getSpeed() const
{
	return mLfo->getParam(Lfo::Param_t::freqInHz);
}

ModulationIf::Shape ModulationBase::getShape() const
{
	return ModulationIf::Shape();
}

void ModulationBase::updateLfoDc(float dAmp)
{
	auto oldDc = float{ -1.0f * mLfo->getParam(Lfo::dc) };
	auto newDc = float{ oldDc + dAmp };
	mLfo->setParam(Lfo::dc, -1.0f * newDc);
}

Chorus::Chorus(float sampleRate, float maxDelayInMs, float maxDepthInMs) :
	ModulationBase(sampleRate, maxDelayInMs, maxDepthInMs)
{
	setDelay(20);
}

void Chorus::process(const float const* inputBuffer, float* outputBuffer, const int numSamples)
{
	for (int i = 0; i < numSamples; i++) {
		mDelayLine->putPostInc(inputBuffer[i]);
		auto offset = float{ mLfo->process() };
		outputBuffer[i] = mDelayLine->get() + mDelayLine->get(offset);
		mDelayLine->getPostInc();
	}
}
