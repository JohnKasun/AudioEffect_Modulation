#include "Chorus.h"

Chorus::Chorus(float sampleRate, float maxDepthInMs, float maxSpeedInHz, int maxNumVoices) {
  if (sampleRate <= 0.0f) throw Exception("Invalid Samplerate");
  if (maxDepthInMs < 0.0f) throw Exception("Invalid Maximum Depth");
  if (maxSpeedInHz < 0.0f) throw Exception("Invalid Maximum Speed");
  if (maxDepthInMs < 0) throw Exception("Invalid Number of Voices");

  mSampleRate = sampleRate;
  mMaxDepthInMs = maxDepthInMs;
  mMaxSpeedInHz = maxSpeedInHz;
  mMaxNumVoices = maxNumVoices;

  auto phaseIncrement = float{2.0f * static_cast<float>(M_PI) / maxNumVoices};
  auto phaseAccum = float{static_cast<float>(M_PI / 2.0f)};
  for (auto i = 0; i < maxNumVoices; i++) {
    mLfo.emplace_back(new Lfo(mSampleRate));
    mLfo.back()->setParam(Lfo::Param_t::phaseInRadians, phaseAccum);
    phaseAccum += phaseIncrement;
  }

  auto delayInSamp = int{CUtil::float2int<int>(mSampleRate * 20.0f / 1000.0f)};
  auto maxDepthInSamp = int{CUtil::float2int<int>(mSampleRate * maxDepthInMs / 1000.0f)};
  assert(maxDepthInSamp <= delayInSamp);
  mDelayLine.reset(new CRingBuffer<float>(1 + delayInSamp + maxDepthInSamp));
  mDelayLine->setWriteIdx(delayInSamp + maxDepthInSamp / 2);
}

Chorus::~Chorus() {}

void Chorus::setDepth(float newDepthInMs) {
  if (newDepthInMs < 0.0f || newDepthInMs > mMaxDepthInMs) throw Exception("Invalid Depth Parameter");

  auto newDepthInSamp = float{newDepthInMs * mSampleRate / 1000.0f};
  auto newAmplitude = float{newDepthInSamp / 2.0f};
  for (auto& lfo : mLfo) {
    lfo->setParam(Lfo::Param_t::amplitude, newAmplitude);
  }
}

void Chorus::setSpeed(float newSpeedInHz) {
  if (newSpeedInHz < 0.0f || newSpeedInHz > mMaxSpeedInHz) throw Exception("Invalid Speed Parameter");

  for (auto& lfo : mLfo) {
    lfo->setParam(Lfo::Param_t::freqInHz, newSpeedInHz);
  }
}

void Chorus::setNumVoices(int newNumVoices) {
  if (newNumVoices < 0 || newNumVoices > mMaxNumVoices) throw Exception("Invalid Number Of Voices");

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

void Chorus::process(const float const* inputBuffer, float* outputBuffer, const int numSamples) {
  if (!inputBuffer) throw Exception("Input Buffer is NULL");
  if (!outputBuffer) throw Exception("Output Buffer is NULL");
  if (numSamples < 0) throw Exception("Invalid Buffer Length");

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
