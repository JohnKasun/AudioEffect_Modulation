#pragma once

#include <memory>
#include <vector>

#include "catch.hpp"
#include "Chorus.h"
#include "ErrorDef.h"
#include "Synthesis.h"
#include "CatchUtil.h"

TEST_CASE("Chorus") {
	std::unique_ptr<Chorus> chorus;
	std::unique_ptr<float> inputBuffer;
	std::unique_ptr<float> outputBuffer;
	std::vector<float> groundBuffer;
	SECTION("Zero Delay") {

	}
	SECTION("Zero Depth") {

	}
	SECTION("Zero Speed") {

	}
	chorus.reset();
	inputBuffer.reset();
	outputBuffer.reset();
	groundBuffer.clear();
}