#pragma once

#include <memory>
#include <vector>

#include "Lfo.h"
#include "RingBuffer.h"
#include "Exception.h"

class Chorus {
 public:
  enum class Shape { Sine, Triangle };
  Chorus(float sampleRate);
  ~Chorus() = default;

  void setDepth(float newDepthInMs);
  void setSpeed(float newSpeedInHz);
  void setNumVoices(int newNumVoices);
  void setShape(Chorus::Shape newShape);
  void setGain(float newGain);
  void setMix(float newMix);

  float getDepth() const;
  float getSpeed() const;
  int getNumVoices() const;
  Chorus::Shape getShape() const;
  float getGain() const;
  float getMix() const;

  void process(const float const* inputBuffer, float* outputBuffer, const int numSamples);

  static const float MaxDepthInMs;
  static const float MaxSpeedInHz;
  static const int MaxNumVoices;
 protected:
  float mSampleRate;

  int mVoicesParam = 1.0f;
  float mGainParam = 1.0f;
  float mMixParam = 0.5f;
  std::vector<std::unique_ptr<Lfo>> mLfo;
  std::unique_ptr<CRingBuffer<float>> mDelayLine;

  float processLfos();

  static const float DelayInMs;
};