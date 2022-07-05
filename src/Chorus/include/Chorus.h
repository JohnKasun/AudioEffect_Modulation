#pragma once

#include <vector>
#include <memory>

#include "ErrorDef.h"
#include "RingBuffer.h"
#include "Lfo.h"
#include "CombFilter.h"

class Chorus {
public:

	enum class Shape {
		Sine,
		Triangle
	};

	Chorus();
	~Chorus();

	Error_t init(float sampleRate);
	Error_t reset();

	Error_t setDelay(float newDelay);
	Error_t setDepth(float newDepth);
	Error_t setSpeed(float newSpeed);
	Error_t setShape(Chorus::Shape newShape);

	float getDelay() const;
	float getDepth() const;
	float getSpeed() const;
	Chorus::Shape getShape() const;

	Error_t process(const float const* inputBuffer, float* outputBuffer, const int numSamples);
private:
	
	enum class RangedParameter {
		Delay,
		Depth,
		Speed,

		NumRangedParameters
	};

	float mSampleRate = 1.0f;
	bool mIsInitialized = false;
	std::unique_ptr<CRingBuffer<float>> mDelayLine;
	std::unique_ptr<Lfo> mLfo;

	float mParamRanges[static_cast<int>(RangedParameter::NumRangedParameters)][2]{};
	bool isParamInRange(Chorus::RangedParameter param, float value) const;
	void updateLfoDc(float dAmp);
};