#pragma once

#include <memory>
#include <vector>

#include "Lfo.h"
#include "RingBuffer.h"
#include "Exception.h"

class Chorus {
 public:
  enum class Shape { Sine, Triangle };
  Chorus(float sampleRate, float maxDepthInMs, float maxSpeedInHz, int maxNumVoices);
  ~Chorus() = default;

  void setDepth(float newDepthInMs);
  void setSpeed(float newSpeedInHz);
  void setNumVoices(int newNumVoices);
  void setShape(Chorus::Shape newShape);

  float getDepth() const;
  float getSpeed() const;
  int getNumVoices() const;
  Chorus::Shape getShape() const;

  void process(const float const* inputBuffer, float* outputBuffer, const int numSamples);
 protected:
  float mSampleRate;
  float mMaxDepthInMs;
  float mMaxSpeedInHz;
  int mMaxNumVoices;

  int mVoicesParam = 1.0f;
  std::vector<std::unique_ptr<Lfo>> mLfo;
  std::unique_ptr<CRingBuffer<float>> mDelayLine;

  float processLfos();

  static const float DelayInMs;
};