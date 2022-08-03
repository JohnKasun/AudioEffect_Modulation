#include "ModulationBase.h"

ModulationBase::ModulationBase(float sampleRate, int numLfos) {
  assert(numLfos > 0);
  mSampleRate = sampleRate;
  auto phaseIncrement = float{2.0f * static_cast<float>(M_PI) / numLfos};
  auto phaseAccum = float{static_cast<float>(M_PI / 2.0f)};
  for (auto i = 0; i < numLfos; i++) {
    mLfo.emplace_back(new Lfo(mSampleRate));
    mLfo.back()->setParam(Lfo::Param_t::phaseInRadians, phaseAccum);
    phaseAccum += phaseIncrement;
  }
}

ModulationBase::~ModulationBase() {
  mLfo.clear();
  mSampleRate = 1.0f;
}

void ModulationBase::setDepth(float newDepthInMs) {
  auto newDepthInSamp = float{newDepthInMs * mSampleRate / 1000.0f};
  auto newAmplitude = float{newDepthInSamp / 2.0f};
  for (auto& lfo : mLfo) {
    lfo->setParam(Lfo::Param_t::amplitude, newAmplitude);
  }
}

void ModulationBase::setSpeed(float newSpeedInHz) {
  for (auto& lfo : mLfo) {
    lfo->setParam(Lfo::Param_t::freqInHz, newSpeedInHz);
  }
}

void ModulationBase::setShape(ModulationIf::Shape newShape) {
  Lfo::Waveform_t newLfoWaveform;
  switch (newShape) {
    case ModulationIf::Shape::Sine:
      newLfoWaveform = Lfo::Waveform_t::Sine;
      break;
    default:
      newLfoWaveform = Lfo::Waveform_t::Triangle;
  }
  for (auto& lfo : mLfo) {
    lfo->setWaveform(newLfoWaveform);
  }
}

float ModulationBase::getDepth() const {
  return mLfo.back()->getParam(Lfo::Param_t::amplitude) * 2.0f / mSampleRate * 1000.0f;
}

float ModulationBase::getSpeed() const {
  return mLfo.back()->getParam(Lfo::Param_t::freqInHz);
}

ModulationIf::Shape ModulationBase::getShape() const {
  switch (mLfo.back()->getWaveform()) {
    case Lfo::Waveform_t::Sine:
      return ModulationIf::Shape::Sine;
    default:
      return ModulationIf::Shape::Triangle;
  }
}

const float Chorus::DelayInMs = 20.0f;

Chorus::Chorus(float sampleRate, float maxDepthInMs, int numVoices) : ModulationBase(sampleRate, numVoices) {
  auto delayInSamp = int{CUtil::float2int<int>(mSampleRate * DelayInMs / 1000.0f)};
  auto maxDepthInSamp = int{CUtil::float2int<int>(mSampleRate * maxDepthInMs / 1000.0f)};
  assert(maxDepthInSamp <= delayInSamp);
  mDelayLine.reset(new CRingBuffer<float>(1 + delayInSamp + maxDepthInSamp));
  mDelayLine->setWriteIdx(delayInSamp + maxDepthInSamp / 2);
}

void Chorus::process(const float const* inputBuffer, float* outputBuffer, const int numSamples) {
  for (int i = 0; i < numSamples; i++) {
    mDelayLine->putPostInc(inputBuffer[i]);
    auto delaySum = float{0.0f};
    for (auto& lfo : mLfo) {
      auto offset = float{lfo->process()};
      delaySum += mDelayLine->get(offset);
    }
    outputBuffer[i] = (inputBuffer[i] + delaySum) / (mLfo.size() + 1);
    mDelayLine->getPostInc();
  }
}

Flanger::Flanger(float sampleRate, float maxDepthInMs) : Chorus(sampleRate, maxDepthInMs, 1) {
  mDelayLine->setWriteIdx(CUtil::float2int<int>(mSampleRate * maxDepthInMs / 1000.0f) / 2);
}