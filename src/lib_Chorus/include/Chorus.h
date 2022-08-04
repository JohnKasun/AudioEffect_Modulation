#pragma once

#include <memory>
#include <vector>

#include "Lfo.h"
#include "RingBuffer.h"
#include "Exception.h"

class Chorus {
 public:
  enum class Shape { Sine, Triangle };
  Chorus(float sampleRate, float maxDepthInMs, int maxNumVoices);
  ~Chorus();

  void setDepth(float newDepthInMs);
  void setSpeed(float newSpeedInHz);
  void setShape(Chorus::Shape newShape);

  float getDepth() const;
  float getSpeed() const;
  Chorus::Shape getShape() const;

  void process(const float const* inputBuffer, float* outputBuffer, const int numSamples);

 protected:
  float mSampleRate;
  std::vector<std::unique_ptr<Lfo>> mLfo;
  std::unique_ptr<CRingBuffer<float>> mDelayLine;
};