#pragma once

#include <memory>
#include <vector>

#include "catch.hpp"
#include "ErrorDef.h"
#include "Synthesis.h"
#include "CatchUtil.h"
#include "Vector.h"
#include "ModulationIf.h"

TEST_CASE("Chorus") {
	auto sampleRate = float{ 44100 };
	auto numSamples = int{ 10000 };
	auto chorus = std::make_unique<ModulationIf>();
	chorus->init(sampleRate, ModulationIf::Type::Chorus);
	auto inputBuffer = std::make_unique<float[]>(numSamples);
	auto outputBuffer = std::make_unique<float[]>(numSamples);
	auto groundBuffer = std::make_unique<float[]>(numSamples);
	auto depthParam = float{ 0 };
	auto speedParam = float{ 0 };
	SECTION("Zero Depth") {
		speedParam = 1;
		CSynthesis::generateSine(inputBuffer.get(), 440, sampleRate, numSamples);
		auto delayInSamp = int{ CUtil::float2int<int>(chorus->getDelay() * sampleRate / 1000)};
		CVectorFloat::copy(groundBuffer.get(), inputBuffer.get(), numSamples);
		CVectorFloat::add_I(groundBuffer.get() + delayInSamp, inputBuffer.get(), numSamples - delayInSamp);
		chorus->setDepth(depthParam);
		chorus->setSpeed(speedParam);
		chorus->process(inputBuffer.get(), outputBuffer.get(), numSamples);
		CatchUtil::compare(outputBuffer.get(), groundBuffer.get(), numSamples);
	}
	SECTION("Zero Speed") {
		depthParam = 10;
		CSynthesis::generateSine(inputBuffer.get(), 440, sampleRate, numSamples);
		auto delayInSamp = int{ CUtil::float2int<int>(chorus->getDelay() * sampleRate / 1000)};
		CVectorFloat::copy(groundBuffer.get(), inputBuffer.get(), numSamples);
		CVectorFloat::add_I(groundBuffer.get() + delayInSamp, inputBuffer.get(), numSamples - delayInSamp);
		chorus->setDepth(depthParam);
		chorus->setSpeed(speedParam);
		chorus->process(inputBuffer.get(), outputBuffer.get(), numSamples);
		CatchUtil::compare(outputBuffer.get(), groundBuffer.get(), numSamples);
	}
	SECTION("Update Delay") {
		chorus->setDepth(5);
		REQUIRE(chorus->getDelay() == 20);
		REQUIRE(chorus->getDepth() == 5);
		chorus->setDepth(20);
		REQUIRE(chorus->getDelay() == 20);
		REQUIRE(chorus->getDepth() == 20);
	}
	chorus.reset();
	inputBuffer.reset();
	outputBuffer.reset();
	groundBuffer.reset();
}