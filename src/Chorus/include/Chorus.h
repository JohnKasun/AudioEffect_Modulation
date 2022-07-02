#pragma once

#include "ErrorDef.h"
#include "RingBuffer.h"
#include "Lfo.h"

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

	Error_t setDepth(float newDepth);
	Error_t setSpeed(float newSpeed);
	Error_t setShape(Chorus::Shape newShape);

	float getDepth() const;
	float getSpeed() const;
	Chorus::Shape getShape() const;

private:

	enum class RangedParameters {
		Depth,
		Speed,
		Shape,

		NumParameters
	};
};