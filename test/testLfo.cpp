#pragma once

#include "catch.hpp"
#include "Lfo.h"
#include "CatchUtil.h"
#include "Synthesis.h"
#include "Vector.h"
#include "DataFileIO.h"

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
	SECTION("Print Triangle") {
		freq = float{ 440.0f };
		amp = float{ 2.0f };
		auto dc = float{ 2.0f };
		auto phase = M_PI / 2.0f;
		lfo->setParam(Lfo::Param_t::freqInHz, freq);
		lfo->setParam(Lfo::Param_t::amplitude, amp);
		lfo->setParam(Lfo::Param_t::dc, dc);
		lfo->setParam(Lfo::Param_t::phaseInRadians, phase);
		lfo->setWaveform(Lfo::Waveform_t::Triangle);
		REQUIRE(lfo->getWaveform() == Lfo::Waveform_t::Triangle);
		for (int i = 0; i < numSamples; i++) {
			outputBuffer.get()[i] = lfo->process();
		}
		DataFile::printBuffer("C:/Users/JohnK/Documents/ASE/AudioEffect_Chorus/build/TriangleLfo.csv", outputBuffer.get(), numSamples);
	}
	lfo.reset();
	outputBuffer.reset();
	groundBuffer.reset();
}