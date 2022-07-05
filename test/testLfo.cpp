#pragma once

#include "catch.hpp"
#include "Lfo.h"
#include "CatchUtil.h"
#include "Synthesis.h"
#include "Vector.h"

TEST_CASE("Lfo") {
	auto sampleRate = float{ 48000 };
	auto numSamples = int{ 1000 };
	auto lfo = std::make_unique<Lfo>(sampleRate);
	auto outputBuffer = std::make_unique<float[]>(numSamples);
	auto groundBuffer = std::make_unique<float[]>(numSamples);
	auto freq = float{ 1.0f };
	auto amp = float{ 1 };
	lfo->setParam(Lfo::freqInHz, freq);
	lfo->setParam(Lfo::amplitude, amp);
	SECTION("Freq - Amp") {
		CSynthesis::generateSine(groundBuffer.get(), freq, sampleRate, numSamples, amp);
		for (int i = 0; i < numSamples; i++) {
			outputBuffer.get()[i] = lfo->process();
		}
		CatchUtil::compare(outputBuffer.get(), groundBuffer.get(), numSamples);
	}
	SECTION("Freq - Amp - Phase") {
		auto phase = float{ static_cast<float>(M_PI) };
		lfo->setParam(Lfo::phaseInRadians, phase);
		CSynthesis::generateSine(groundBuffer.get(), freq, sampleRate, numSamples, amp, phase);
		for (int i = 0; i < numSamples; i++) {
			outputBuffer.get()[i] = lfo->process();
		}
		CatchUtil::compare(outputBuffer.get(), groundBuffer.get(), numSamples);
	}
	SECTION("Freq - Amp - Phase - DC") {
		auto phase = float{ static_cast<float>(M_PI) };
		auto dc = float{ 3 };
		lfo->setParam(Lfo::phaseInRadians, phase);
		lfo->setParam(Lfo::dc, dc);
		CSynthesis::generateSine(groundBuffer.get(), freq, sampleRate, numSamples, amp, phase);
		CVectorFloat::addC_I(groundBuffer.get(), dc, numSamples);
		for (int i = 0; i < numSamples; i++) {
			outputBuffer.get()[i] = lfo->process();
		}
		CatchUtil::compare(outputBuffer.get(), groundBuffer.get(), numSamples);
	}
	lfo.reset();
	outputBuffer.reset();
	groundBuffer.reset();
}