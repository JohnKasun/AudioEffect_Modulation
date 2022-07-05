#pragma once

#include <memory>
#include <vector>

#include "catch.hpp"
#include "Chorus.h"
#include "ErrorDef.h"
#include "Synthesis.h"
#include "CatchUtil.h"

TEST_CASE("Chorus") {
	auto sampleRate = float{ 44100 };
	auto numSamples = int{ 10000 };
	auto chorus = std::make_unique<Chorus>();
	chorus->init(sampleRate);
	auto inputBuffer = std::make_unique<float[]>(numSamples);
	auto outputBuffer = std::make_unique<float[]>(numSamples);
	auto groundBuffer = std::make_unique<float[]>(numSamples);
	auto delayParam = float{ 0 };
	auto depthParam = float{ 0 };
	auto speedParam = float{ 0 };
	SECTION("Zero Delay") {
		depthParam = 10;
		speedParam = 1;
		CSynthesis::generateDc(inputBuffer.get(), numSamples, 1);
		CSynthesis::generateDc(groundBuffer.get(), numSamples, 2);
		chorus->setDelay(delayParam);
		chorus->setDepth(depthParam);
		chorus->setSpeed(speedParam);
		chorus->process(inputBuffer.get(), outputBuffer.get(), numSamples);
		CatchUtil::compare(outputBuffer.get(), groundBuffer.get(), numSamples);
	}
	SECTION("Zero Depth") {
		//delayParam = 10;
		//speedParam = 3;
		//CSynthesis::generateSine(inputBuffer.get(), 440, sampleRate, numSamples);
	}
	SECTION("Zero Speed") {

	}
	chorus.reset();
	inputBuffer.reset();
	outputBuffer.reset();
	groundBuffer.reset();
}