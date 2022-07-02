#pragma once

#include <vector>
#include <memory>

#include "ErrorDef.h"
#include "RingBuffer.h"
#include "Lfo.h"

class Chorus {
public:

	enum class Shape {
		Sine,
		Triangle
	};

	enum class Parameter {
		Depth,
		Speed,
		Shape
	};

	Chorus();
	~Chorus();

	Error_t init(float sampleRate);
	Error_t reset();

	Error_t setParameter(Chorus::Parameter param, auto value);
	auto getParameter(Chorus::Parameter param) const;

	Error_t process(const float const* inputBuffer, float const* outputBuffer, const int numSamples);
private:
	
	const float mMaxDelayInSec = 0.6;
	const int mNumVoices = 3;

	float mSampleRate = 1.0f;
	bool mIsInitialized = false;
	std::unique_ptr<CRingBuffer<float>> mDelayLine;
	std::vector<Lfo> mLfos;

	bool isParamInRange(Chorus::Parameter param, auto value) const;
};