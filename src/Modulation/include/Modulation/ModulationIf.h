#pragma once

#include "ErrorDef.h"

class ModulationBase;
class ModulationIf {
public:
	enum class Type {
		Chorus,
		Flanger,
		Phaser
	};
	enum class Shape {
		Sine,
		Triangle
	};
	ModulationIf();
	~ModulationIf();

	Error_t init(float sampleRate, Type type);
	Error_t reset();

	Error_t setDepth(float newDepth);
	Error_t setSpeed(float newSpeed);
	Error_t setShape(Shape newShape);

	float getDepth() const;
	float getSpeed() const;
	Shape getShape() const;

	static float getMaxDepth();
	static float getMaxSpeed();

	Error_t process(const float const* inputBuffer, float* outputBuffer, const int numSamples);
private:
	enum class RangedParameter {
		Depth,
		Speed,

		NumRangedParameters
	};

	float mParamRanges[static_cast<int>(RangedParameter::NumRangedParameters)][2]{};
	bool isParamInRange(RangedParameter param, float value) const;

	ModulationBase* mMod = nullptr;
	bool mIsInitialized = false;
	Type mCurrentType;

	static const float MaxDepthInMs;
	static const float MaxSpeedInMs;
};