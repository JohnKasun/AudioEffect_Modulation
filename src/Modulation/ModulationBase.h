#pragma once

#include <vector>
#include <memory>

#include "ModulationIf.h"
#include "RingBuffer.h"
#include "Lfo.h"

class ModulationBase {
public:
	ModulationBase(float sampleRate, float maxDelayInMs, float maxDepthInMs);
	~ModulationBase();

	void setDelay(float newDelayInMs);
	void setDepth(float newDepthInMs);
	void setSpeed(float newSpeedInHz);
	void setShape(ModulationIf::Shape newShape);

	float getDelay() const;
	float getDepth() const;
	float getSpeed() const;
	ModulationIf::Shape getShape() const;

	virtual void process(const float const* inputBuffer, float* outputBuffer, const int numSamples) = 0;
protected:
	float mSampleRate = 1.0f;
	std::unique_ptr<CRingBuffer<float>> mDelayLine;
	std::unique_ptr<Lfo> mLfo;

	void updateLfoDc(float dAmp);
};

class Chorus : public ModulationBase {
public:

	Chorus(float sampleRate, float maxDelayInMs, float maxDepthInMs);
	virtual ~Chorus() = default;

	virtual void process(const float const* inputBuffer, float* outputBuffer, const int numSamples) override;
};