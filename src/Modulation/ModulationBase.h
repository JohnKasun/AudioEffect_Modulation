#pragma once

#include <vector>
#include <memory>

#include "ModulationIf.h"
#include "RingBuffer.h"
#include "Lfo.h"

class ModulationBase {
public:
	ModulationBase(float sampleRate, int numLfos);
	~ModulationBase();

	void setDepth(float newDepthInMs);
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
	Chorus(float sampleRate, float maxDepthInMs);
	virtual ~Chorus() = default;

	virtual void process(const float const* inputBuffer, float* outputBuffer, const int numSamples) override;
protected:
	Chorus(float sampleRate, float maxDepthInMs, int numLfos);
	std::unique_ptr<CRingBuffer<float>> mDelayLine;
private:
	const float mDelayInMs = 20.0f;
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

	virtual void process(const float const* inputBuffer, float* outputBuffer, const int numSamples) override;
};