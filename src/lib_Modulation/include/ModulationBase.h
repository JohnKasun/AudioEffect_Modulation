#pragma once

#include <memory>
#include <vector>

#include "Lfo.h"
#include "ModulationIf.h"
#include "RingBuffer.h"
#include "APFilter.h"

class ModulationBase {
 public:
  ModulationBase(float sampleRate, int numLfos);
  ~ModulationBase();

  virtual void setDepth(float newDepthInMs);
  void setSpeed(float newSpeedInHz);
  void setShape(ModulationIf::Shape newShape);

  float getDepth() const;
  float getSpeed() const;
  ModulationIf::Shape getShape() const;

  virtual void process(const float const* inputBuffer, float* outputBuffer, const int numSamples) = 0;

 protected:
  float mSampleRate = 1.0f;
  std::vector<std::unique_ptr<Lfo>> mLfo;
};

class Chorus : public ModulationBase {
 public:
  Chorus(float sampleRate, float maxDepthInMs, int numVoices = 3);
  virtual ~Chorus() = default;

  virtual void process(const float const* inputBuffer, float* outputBuffer, const int numSamples) override;

 protected:
  std::unique_ptr<CRingBuffer<float>> mDelayLine;

 private:
  static const float DelayInMs;
};

class Flanger : public Chorus {
 public:
  Flanger(float sampleRate, float maxDepthInMs);
  virtual ~Flanger() = default;
};

class Phaser : public ModulationBase {
 public:
  Phaser(float sampleRate);
  virtual ~Phaser() = default;

  virtual void setDepth(float newDepth) override;
  virtual void process(const float const* inputBuffer, float* outputBuffer, const int numSamples) override;

 private:
  std::vector<std::unique_ptr<APFilter>> mFilter;
};