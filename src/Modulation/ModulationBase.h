#pragma once

#include <vector>
#include <memory>

#include "ModulationIf.h"
#include "RingBuffer.h"
#include "Lfo.h"

class ModulationBase {
public:
	ModulationBase(float sampleRate, float maxDepthInMs, float delayInMs = 0.0f, int numLfos = 1);
	~ModulationBase();

	void setDepth(float newDepthInMs);
	void setSpeed(float newSpeedInHz);
	void setShape(ModulationIf::Shape newShape);

	float getDelay() const;
	float getDepth() const;
	float getSpeed() const;
	ModulationIf::Shape getShape() const;

	virtual void process(const float const* inputBuffer, float* outputBuffer, const int numSamples);
protected:
	float mSampleRate = 1.0f;
	std::unique_ptr<CRingBuffer<float>> mDelayLine;
	std::vector<std::unique_ptr<Lfo>> mLfo;

	void updateLfoDc(std::unique_ptr<Lfo>& lfo, float dAmp);
};

class Chorus : public ModulationBase {
public:
	Chorus(float sampleRate, float maxDepthInMs);
	virtual ~Chorus() = default;
};

class Flanger : public ModulationBase {
public:
	Flanger(float sampleRate, float maxDepthInMs);
	virtual ~Flanger() = default;
};

class Phaser : public ModulationBase {
public:
	Phaser(float sampleRate, float maxDepthInMs);
	virtual ~Phaser() = default;

	virtual void process(const float const* inputBuffer, float* outputBuffer, const int numSamples) override;
};