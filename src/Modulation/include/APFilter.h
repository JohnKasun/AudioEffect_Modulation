#pragma once

#include "RingBuffer.h"

class APFilter {
public:
	APFilter();
	~APFilter();

	void setGain(float newGain);
	float getGain() const;
	float process(float input);
private:
	CRingBuffer<float> mInputDelayLine;
	CRingBuffer<float> mOutputDelayLine;
	float mGain = 1.0f;
};