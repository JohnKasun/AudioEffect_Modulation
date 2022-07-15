#include "ModulationBase.h"

ModulationBase::ModulationBase(float sampleRate, float maxDepthInMs, float delayInMs, int numLfos)
{
	mSampleRate = sampleRate;
	auto delayInSamp = int{ CUtil::float2int<int>(mSampleRate * delayInMs / 1000.0f) };
	auto maxDepthInSamp = int{ CUtil::float2int<int>(mSampleRate * maxDepthInMs / 1000.0f) };
	mDelayLine.reset(new CRingBuffer<float>(1 + delayInSamp + maxDepthInSamp));
	auto phaseIncrement = float{ 2.0f * static_cast<float>(M_PI) / numLfos };
	auto phaseAccum = float{ static_cast<float>(M_PI / 2.0f) };
	for (auto i = 0; i < numLfos; i++) {
		mLfo.emplace_back(new Lfo(mSampleRate));
		mLfo.back()->setParam(Lfo::Param_t::phaseInRadians, phaseAccum);
		mLfo.back()->setParam(Lfo::Param_t::dc, -1.0f * (delayInSamp + mLfo.back()->getParam(Lfo::Param_t::amplitude)));
		phaseAccum += phaseIncrement;
	}
}

ModulationBase::~ModulationBase()
{
	mDelayLine.reset();
	mLfo.clear();
	mSampleRate = 1.0f;
}

void ModulationBase::setDepth(float newDepthInMs)
{
	auto newDepthInSamp = float{ newDepthInMs * mSampleRate / 1000.0f };
	auto newAmplitude = float{ newDepthInSamp / 2.0f };
	for (auto& lfo : mLfo) {
		updateLfoDc(lfo, newAmplitude - lfo->getParam(Lfo::amplitude));
		lfo->setParam(Lfo::Param_t::amplitude, newAmplitude);
	}
}

void ModulationBase::setSpeed(float newSpeedInHz)
{
	for (auto& lfo : mLfo) {
		lfo->setParam(Lfo::Param_t::freqInHz, newSpeedInHz);
	}
}

void ModulationBase::setShape(ModulationIf::Shape newShape)
{
	return void();
}

float ModulationBase::getDelay() const
{
	return ((-1.0f * mLfo.back()->getParam(Lfo::Param_t::dc)) - mLfo.back()->getParam(Lfo::Param_t::amplitude)) / mSampleRate * 1000.0f;
}

float ModulationBase::getDepth() const
{
	return mLfo.back()->getParam(Lfo::Param_t::amplitude) * 2.0f / mSampleRate * 1000.0f;
}

float ModulationBase::getSpeed() const
{
	return mLfo.back()->getParam(Lfo::Param_t::freqInHz);
}

ModulationIf::Shape ModulationBase::getShape() const
{
	return ModulationIf::Shape();
}

void ModulationBase::updateLfoDc(std::unique_ptr<Lfo>& lfo, float dAmp)
{
	auto oldDc = float{ -1.0f * lfo->getParam(Lfo::dc) };
	auto newDc = float{ oldDc + dAmp };
	lfo->setParam(Lfo::dc, -1.0f * newDc);
}

void ModulationBase::process(const float const* inputBuffer, float* outputBuffer, const int numSamples)
{
	for (int i = 0; i < numSamples; i++) {
		mDelayLine->putPostInc(inputBuffer[i] / (mLfo.size() + 1));
		auto sum = float{ 0.0f };
		for (auto& lfo : mLfo) {
			auto offset = float{ lfo->process() };
			sum += mDelayLine->get(offset);
		}
		outputBuffer[i] = mDelayLine->get() + sum;
		mDelayLine->getPostInc();
	}
}

Chorus::Chorus(float sampleRate, float maxDepthInMs) :
	ModulationBase(sampleRate, maxDepthInMs, 20.0f, 3)
{
}

Flanger::Flanger(float sampleRate, float maxDepthInMs) : 
	ModulationBase(sampleRate, maxDepthInMs)
{
}

Phaser::Phaser(float sampleRate, float maxDepthInMs) :
	ModulationBase(sampleRate, maxDepthInMs)
{
}

void Phaser::process(const float const* inputBuffer, float* outputBuffer, const int numSamples)
{
}
