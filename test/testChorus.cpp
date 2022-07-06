#pragma once

#include <memory>
#include <vector>

#include "catch.hpp"
#include "Chorus.h"
#include "ErrorDef.h"
#include "Synthesis.h"
#include "CatchUtil.h"
#include "Vector.h"

TEST_CASE("Chorus") {
	auto sampleRate = float{ 44100 };
	auto numSamples = int{ 10000 };
	auto chorus = std::make_unique<Chorus>();
	chorus->init(sampleRate);
	auto inputBuffer = std::make_unique<float[]>(numSamples);
	auto outputBuffer = std::make_unique<float[]>(numSamples);
	auto groundBuffer = std::make_unique<float[]>(numSamples);
	auto const delay = int{ 1 + CUtil::float2int<int>(sampleRate * 0.02 / 2.0f) };
	auto depthParam = float{ 0 };
	auto speedParam = float{ 0 };
	SECTION("Zero Depth") {
		speedParam = 1;
		CSynthesis::generateSine(inputBuffer.get(), 440, sampleRate, numSamples);
		CVectorFloat::copy(groundBuffer.get(), inputBuffer.get(), numSamples);
		CVectorFloat::add_I(groundBuffer.get() + delay, inputBuffer.get(), numSamples - delay);
		REQUIRE(chorus->setDepth(depthParam) == Error_t::kNoError);
		REQUIRE(chorus->setSpeed(speedParam) == Error_t::kNoError);
		REQUIRE(chorus->process(inputBuffer.get(), outputBuffer.get(), numSamples) == Error_t::kNoError);
		CatchUtil::compare(outputBuffer.get(), groundBuffer.get(), numSamples);
	}
	SECTION("Zero Speed") {
		depthParam = 10;
		CSynthesis::generateSine(inputBuffer.get(), 440, sampleRate, numSamples);
		CVectorFloat::copy(groundBuffer.get(), inputBuffer.get(), numSamples);
		CVectorFloat::add_I(groundBuffer.get() + delay, inputBuffer.get(), numSamples - delay);
		REQUIRE(chorus->setDepth(depthParam) == Error_t::kNoError);
		REQUIRE(chorus->setSpeed(speedParam) == Error_t::kNoError);
		REQUIRE(chorus->process(inputBuffer.get(), outputBuffer.get(), numSamples) == Error_t::kNoError);
		CatchUtil::compare(outputBuffer.get(), groundBuffer.get(), numSamples);
	}
	chorus.reset();
	inputBuffer.reset();
	outputBuffer.reset();
	groundBuffer.reset();
}