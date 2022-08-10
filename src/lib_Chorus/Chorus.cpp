#include "Chorus.h"

const float Chorus::DelayInMs = 20.0f;
const float Chorus::MaxDepthInMs = 20.0f;
const float Chorus::MaxSpeedInHz = 1.0f;
const int Chorus::MaxNumVoices = 5;

Chorus::Chorus(float sampleRate) {
  if (sampleRate <= 0.0f) throw Exception("Invalid Samplerate");
  mSampleRate = sampleRate;

  auto phaseIncrement = float{2.0f * static_cast<float>(M_PI) / MaxNumVoices};
  auto phaseAccum = 0.0f;
  for (auto i = 0; i < MaxNumVoices; i++) {
    mLfo.emplace_back(new Lfo(mSampleRate));
    mLfo.back()->setParam(Lfo::Param_t::phaseInRadians, phaseAccum);
    phaseAccum += phaseIncrement;
  }

  auto delayInSamp = int{CUtil::float2int<int>(mSampleRate * DelayInMs / 1000.0f)};
  auto maxDepthInSamp = int{CUtil::float2int<int>(mSampleRate * MaxDepthInMs / 1000.0f)};
  mDelayLine.reset(new CRingBuffer<float>(1 + delayInSamp + maxDepthInSamp));
  mDelayLine->setWriteIdx(delayInSamp + maxDepthInSamp / 2);
}

void Chorus::setDepth(float newDepthInMs) {
  if (newDepthInMs < 0.0f || newDepthInMs > MaxDepthInMs) throw Exception("Invalid Depth Parameter");

  auto newDepthInSamp = float{newDepthInMs * mSampleRate / 1000.0f};
  auto newAmplitude = float{newDepthInSamp / 2.0f};
  for (auto& lfo : mLfo) {
    lfo->setParam(Lfo::Param_t::amplitude, newAmplitude);
  }
}

void Chorus::setSpeed(float newSpeedInHz) {
  if (newSpeedInHz < 0.0f || newSpeedInHz > MaxSpeedInHz) throw Exception("Invalid Speed Parameter");

  for (auto& lfo : mLfo) {
    lfo->setParam(Lfo::Param_t::freqInHz, newSpeedInHz);
  }
}

void Chorus::setNumVoices(int newNumVoices) {
  if (newNumVoices < 0 || newNumVoices > MaxNumVoices) throw Exception("Invalid Number Of Voices");

  mVoicesParam = newNumVoices;
}

void Chorus::setShape(Chorus::Shape newShape) {
  Lfo::Waveform_t newLfoWaveform;
  switch (newShape) {
    case Chorus::Shape::Sine:
      newLfoWaveform = Lfo::Waveform_t::Sine;
      break;
    default:
      newLfoWaveform = Lfo::Waveform_t::Triangle;
  }
  for (auto& lfo : mLfo) {
    lfo->setWaveform(newLfoWaveform);
  }
}

void Chorus::setGain(float newGain) {
  if (newGain < 0.0f || newGain > 5.0f) throw Exception("Invalid Gain Parameter");

  mGainParam = newGain;
}

void Chorus::setMix(float newMix) {
  if (newMix < 0.0f || newMix > 1.0f) throw Exception("Invalid Mix Parameter");

  mMixParam = newMix;
}

float Chorus::getDepth() const {
  return mLfo.back()->getParam(Lfo::Param_t::amplitude) * 2.0f / mSampleRate * 1000.0f;
}

float Chorus::getSpeed() const {
  return mLfo.back()->getParam(Lfo::Param_t::freqInHz); }

int Chorus::getNumVoices() const { return mVoicesParam; }

Chorus::Shape Chorus::getShape() const {
  switch (mLfo.back()->getWaveform()) {
    case Lfo::Waveform_t::Sine:
      return Chorus::Shape::Sine;
    default:
      return Chorus::Shape::Triangle;
  }
}

float Chorus::getGain() const { return mGainParam; }

float Chorus::getMix() const { return mMixParam; }

void Chorus::process(const float const* inputBuffer, float* outputBuffer, const int numSamples) {
  if (!inputBuffer) throw Exception("Input Buffer is NULL");
  if (!outputBuffer) throw Exception("Output Buffer is NULL");
  if (numSamples < 0) throw Exception("Invalid Buffer Length");

  for (int i = 0; i < numSamples; i++) {
    mDelayLine->putPostInc(inputBuffer[i]);
    auto lfoSum = processLfos();
    auto out = (1.0f - mMixParam) * inputBuffer[i] + mMixParam * lfoSum;
    outputBuffer[i] = mGainParam * out;
    mDelayLine->getPostInc();
  }
}

float Chorus::processLfos() {
  auto lfoSum = 0.0f;
  for (auto i = 0; i < mVoicesParam; i++) {
    auto offset = mLfo[i]->process();
    lfoSum += mDelayLine->get(offset);
  }
  for (auto i = mVoicesParam; i < mLfo.size(); i++) {
    mLfo[i]->process();  // Dummy call to keep lfos in sync
  }
  return lfoSum;
}
