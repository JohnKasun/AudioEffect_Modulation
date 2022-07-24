#pragma once

#include <gtest/gtest.h>

#include "DataFileIO.h"
#include "GTestUtil.h"
#include "Lfo.h"
#include "Synthesis.h"
#include "Vector.h"

class LfoTestSuite : public ::testing::Test {
 protected:
  void SetUp() override {
    mSampleRate = 48000.0f;
    mNumSamples = 1000;
    mFreq = 1.0f;
    mAmp = 1.0f;
    mLfo.reset(new Lfo(mSampleRate));
    mOutputBuffer.reset(new float[mNumSamples]{});
    mGroundBuffer.reset(new float[mNumSamples]{});
    mLfo->setParam(Lfo::freqInHz, mFreq);
    mLfo->setParam(Lfo::amplitude, mAmp);
  }
  float mSampleRate;
  int mNumSamples;
  float mFreq;
  float mAmp;
  std::unique_ptr<Lfo> mLfo = nullptr;
  std::unique_ptr<float[]> mOutputBuffer = nullptr;
  std::unique_ptr<float[]> mGroundBuffer = nullptr;
};

TEST_F(LfoTestSuite, FreqAmp) {
  CSynthesis::generateSine(mGroundBuffer.get(), mFreq, mSampleRate, mNumSamples, mAmp);
  for (int i = 0; i < mNumSamples; i++) {
    mOutputBuffer.get()[i] = mLfo->process();
  }
  GTestUtil::compare(mOutputBuffer.get(), mGroundBuffer.get(), mNumSamples);
}

TEST_F(LfoTestSuite, FreqAmpPhase) {
  auto phase = float{static_cast<float>(M_PI)};
  mLfo->setParam(Lfo::phaseInRadians, phase);
  CSynthesis::generateSine(mGroundBuffer.get(), mFreq, mSampleRate, mNumSamples, mAmp, phase);
  for (int i = 0; i < mNumSamples; i++) {
    mOutputBuffer.get()[i] = mLfo->process();
  }
  GTestUtil::compare(mOutputBuffer.get(), mGroundBuffer.get(), mNumSamples);
}

TEST_F(LfoTestSuite, FreqAmpPhaseDC) {
  auto phase = float{static_cast<float>(M_PI)};
  auto dc = float{3};
  mLfo->setParam(Lfo::phaseInRadians, phase);
  mLfo->setParam(Lfo::dc, dc);
  CSynthesis::generateSine(mGroundBuffer.get(), mFreq, mSampleRate, mNumSamples, mAmp, phase);
  CVectorFloat::addC_I(mGroundBuffer.get(), dc, mNumSamples);
  for (int i = 0; i < mNumSamples; i++) {
    mOutputBuffer.get()[i] = mLfo->process();
  }
  GTestUtil::compare(mOutputBuffer.get(), mGroundBuffer.get(), mNumSamples);
}

TEST_F(LfoTestSuite, PrintTriangle) {
  mFreq = 440.0f;
  mAmp = 2.0f;
  auto dc = float{2.0f};
  auto phase = M_PI / 2.0f;
  mLfo->setParam(Lfo::Param_t::freqInHz, mFreq);
  mLfo->setParam(Lfo::Param_t::amplitude, mAmp);
  mLfo->setParam(Lfo::Param_t::dc, dc);
  mLfo->setParam(Lfo::Param_t::phaseInRadians, phase);
  mLfo->setWaveform(Lfo::Waveform_t::Triangle);
  EXPECT_EQ(Lfo::Waveform_t::Triangle, mLfo->getWaveform());
  for (int i = 0; i < mNumSamples; i++) {
    mOutputBuffer.get()[i] = mLfo->process();
  }
  DataFile::printBuffer("C:/Users/JohnK/Documents/ASE/AudioEffect_Chorus/build/TriangleLfo.csv", mOutputBuffer.get(), mNumSamples);
}