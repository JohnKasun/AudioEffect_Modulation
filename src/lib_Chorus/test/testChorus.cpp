#pragma once

#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "ErrorDef.h"
#include "GTestUtil.h"
#include "Chorus.h"
#include "Synthesis.h"
#include "Vector.h"

class ChorusTestSuite : public ::testing::Test {
 protected:
  virtual void SetUp() override {
    mSampleRate = float{44100};
    mNumSamples = int{10000};
    mChorus.reset(new Chorus(mSampleRate));
    mInputBuffer.reset(new float[mNumSamples]{});
    mOutputBuffer.reset(new float[mNumSamples]{});
    mGroundBuffer.reset(new float[mNumSamples]{});
  }
  float mSampleRate;
  int mNumSamples;
  std::unique_ptr<Chorus> mChorus;
  std::unique_ptr<float[]> mInputBuffer;
  std::unique_ptr<float[]> mOutputBuffer;
  std::unique_ptr<float[]> mGroundBuffer;
};
TEST_F(ChorusTestSuite, ZeroDepth) {
  CSynthesis::generateSine(mInputBuffer.get(), 440, mSampleRate, mNumSamples);
  auto delayInSamp = int{CUtil::float2int<int>((Chorus::DelayInMs + Chorus::MaxDepthInMs / 2.0f) * mSampleRate / 1000)};
  CVectorFloat::copy(mGroundBuffer.get(), mInputBuffer.get(), mNumSamples);
  CVectorFloat::add_I(mGroundBuffer.get() + delayInSamp, mInputBuffer.get(), mNumSamples - delayInSamp);
  CVectorFloat::mulC_I(mGroundBuffer.get(), 0.5, mNumSamples);
  try {
    mChorus->setDepth(0.0f);
    mChorus->setSpeed(1.0f);
    mChorus->setShape(Chorus::Shape::Sine);
    mChorus->setNumVoices(1);
    mChorus->setGain(1);
    mChorus->setMix(0.5);
    mChorus->process(mInputBuffer.get(), mOutputBuffer.get(), mNumSamples);
  } catch (Exception& ex) {
    FAIL();
  }
  GTestUtil::compare(mOutputBuffer.get(), mGroundBuffer.get(), mNumSamples);
}

TEST_F(ChorusTestSuite, ZeroSpeed) {
  CSynthesis::generateSine(mInputBuffer.get(), 440, mSampleRate, mNumSamples);
  auto delayInSamp = int{CUtil::float2int<int>((Chorus::DelayInMs + Chorus::MaxDepthInMs / 2.0f) * mSampleRate / 1000)};
  CVectorFloat::copy(mGroundBuffer.get(), mInputBuffer.get(), mNumSamples);
  CVectorFloat::add_I(mGroundBuffer.get() + delayInSamp, mInputBuffer.get(), mNumSamples - delayInSamp);
  CVectorFloat::mulC_I(mGroundBuffer.get(), 0.5, mNumSamples);
  try {
    mChorus->setDepth(20.0f);
    mChorus->setSpeed(0);
    mChorus->setShape(Chorus::Shape::Sine);
    mChorus->setNumVoices(1);
    mChorus->setGain(1);
    mChorus->setMix(0.5);
    mChorus->process(mInputBuffer.get(), mOutputBuffer.get(), mNumSamples);
  } catch (Exception& ex) {
    FAIL();
  }
  GTestUtil::compare(mOutputBuffer.get(), mGroundBuffer.get(), mNumSamples);
}

TEST_F(ChorusTestSuite, Gain) { 
    std::array<float, 3> gains{0.0f, 0.5f, 1.0f};
    for (auto gain : gains) {
      SetUp();
      CSynthesis::generateSine(mInputBuffer.get(), 440, mSampleRate, mNumSamples);
      auto delayInSamp = int{CUtil::float2int<int>((Chorus::DelayInMs + Chorus::MaxDepthInMs / 2.0f) * mSampleRate / 1000)};
      CVectorFloat::copy(mGroundBuffer.get(), mInputBuffer.get(), mNumSamples);
      CVectorFloat::add_I(mGroundBuffer.get() + delayInSamp, mInputBuffer.get(), mNumSamples - delayInSamp);
      CVectorFloat::mulC_I(mGroundBuffer.get(), 0.5, mNumSamples);
      CVectorFloat::mulC_I(mGroundBuffer.get(), gain, mNumSamples);
      try {
        mChorus->setDepth(0);
        mChorus->setSpeed(0);
        mChorus->setShape(Chorus::Shape::Sine);
        mChorus->setNumVoices(1);
        mChorus->setGain(gain);
        mChorus->setMix(0.5);
        mChorus->process(mInputBuffer.get(), mOutputBuffer.get(), mNumSamples);
      } catch (Exception& ex) {
        FAIL();
      }
      GTestUtil::compare(mOutputBuffer.get(), mGroundBuffer.get(), mNumSamples);
    }
}
