#include "ModulationIf.h"
#include "ModulationBase.h"


ModulationIf::ModulationIf()
{
	mParamRanges[static_cast<int>(RangedParameter::Depth)][0] = 0;
	mParamRanges[static_cast<int>(RangedParameter::Depth)][1] = 20;
	mParamRanges[static_cast<int>(RangedParameter::Speed)][0] = 0;
	mParamRanges[static_cast<int>(RangedParameter::Speed)][1] = 1;
}

ModulationIf::~ModulationIf()
{
	reset();
}

Error_t ModulationIf::init(float sampleRate, Type type)
{
	if (sampleRate <= 0.0f)
		return Error_t::kFunctionInvalidArgsError;

	reset();

	auto maxDepth = mParamRanges[static_cast<int>(RangedParameter::Depth)][1];
	switch (type) {
	case Type::Chorus:
		mMod = new Chorus(sampleRate, maxDepth);
		mCurrentType = Type::Chorus;
		break;
	case Type::Flanger:
		mMod = new Flanger(sampleRate, maxDepth);
		mCurrentType = Type::Flanger;
		break;
	case Type::Phaser:
		break;
	default:
		return Error_t::kFunctionInvalidArgsError;
	}
	mIsInitialized = true;
	return Error_t::kNoError;
}

Error_t ModulationIf::reset()
{
	if (mIsInitialized) {
		delete mMod;
		mIsInitialized = false;
	}
	return Error_t::kNoError;
}

Error_t ModulationIf::setDepth(float newDepth)
{
	if (!mIsInitialized)
		return Error_t::kNotInitializedError;
	if (!isParamInRange(ModulationIf::RangedParameter::Depth, newDepth))
		return Error_t::kFunctionInvalidArgsError;

	mMod->setDepth(newDepth);
	return Error_t::kNoError;
}

Error_t ModulationIf::setSpeed(float newSpeed)
{
	if (!mIsInitialized)
		return Error_t::kNotInitializedError;
	if (!isParamInRange(ModulationIf::RangedParameter::Speed, newSpeed))
		return Error_t::kFunctionInvalidArgsError;

	mMod->setSpeed(newSpeed);
	return Error_t::kNoError;
}

Error_t ModulationIf::setShape(Shape newShape)
{
	if (!mIsInitialized)
		return Error_t::kNotInitializedError;
	return Error_t();
}

float ModulationIf::getDepth() const
{
	return mMod->getDepth();
}

float ModulationIf::getSpeed() const
{
	return mMod->getSpeed();
}

ModulationIf::Shape ModulationIf::getShape() const
{
	return Shape();
}

Error_t ModulationIf::process(const float const* inputBuffer, float* outputBuffer, const int numSamples)
{
	if (!mIsInitialized)
		return Error_t::kNotInitializedError;
	if (!inputBuffer || !outputBuffer)
		return Error_t::kMemError;
	if (numSamples < 0)
		return Error_t::kFunctionInvalidArgsError;

	mMod->process(inputBuffer, outputBuffer, numSamples);
	return Error_t::kNoError;
}

bool ModulationIf::isParamInRange(ModulationIf::RangedParameter param, float value) const
{
	return (mParamRanges[static_cast<int>(param)][0] <= value && value <= mParamRanges[static_cast<int>(param)][1]);
}
