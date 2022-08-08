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
    mDepthParam = float{0};
    mSpeedParam = float{0};
    mShapeParam = Chorus::Shape::Sine;
    mVoicesParam = int{1};
    mGainParam = float{1};
    mMixParam = float{0.5};
  }
  float mSampleRate;
  int mNumSamples;
  float mDepthParam;
  float mSpeedParam;
  Chorus::Shape mShapeParam;
  int mVoicesParam;
  float mGainParam;
  float mMixParam;
  std::unique_ptr<Chorus> mChorus;
  std::unique_ptr<float[]> mInputBuffer;
  std::unique_ptr<float[]> mOutputBuffer;
  std::unique_ptr<float[]> mGroundBuffer;
};
TEST_F(ChorusTestSuite, ZeroDepth) {
  mSpeedParam = 1;
  CSynthesis::generateSine(mInputBuffer.get(), 440, mSampleRate, mNumSamples);
  auto delayInSamp = int{CUtil::float2int<int>((Chorus::DelayInMs + Chorus::MaxDepthInMs / 2.0f) * mSampleRate / 1000)};
  CVectorFloat::copy(mGroundBuffer.get(), mInputBuffer.get(), mNumSamples);
  CVectorFloat::add_I(mGroundBuffer.get() + delayInSamp, mInputBuffer.get(), mNumSamples - delayInSamp);
  CVectorFloat::mulC_I(mGroundBuffer.get(), 0.5, mNumSamples);
  try {
    mChorus->setDepth(mDepthParam);
    mChorus->setSpeed(mSpeedParam);
    mChorus->setShape(mShapeParam);
    mChorus->setNumVoices(mVoicesParam);
    mChorus->setGain(mGainParam);
    mChorus->setMix(mMixParam);
    mChorus->process(mInputBuffer.get(), mOutputBuffer.get(), mNumSamples);
  } catch (Exception& ex) {
    FAIL();
  }
  GTestUtil::compare(mOutputBuffer.get(), mGroundBuffer.get(), mNumSamples);
}

TEST_F(ChorusTestSuite, ZeroSpeed) {
  mDepthParam = 20.0f;
  CSynthesis::generateSine(mInputBuffer.get(), 440, mSampleRate, mNumSamples);
  auto delayInSamp = int{CUtil::float2int<int>(Chorus::DelayInMs * mSampleRate / 1000)};
  CVectorFloat::copy(mGroundBuffer.get(), mInputBuffer.get(), mNumSamples);
  CVectorFloat::add_I(mGroundBuffer.get() + delayInSamp, mInputBuffer.get(), mNumSamples - delayInSamp);
  CVectorFloat::mulC_I(mGroundBuffer.get(), 0.5, mNumSamples);
  try {
    mChorus->setDepth(mDepthParam);
    mChorus->setSpeed(mSpeedParam);
    mChorus->setShape(mShapeParam);
    mChorus->setNumVoices(mVoicesParam);
    mChorus->setGain(mGainParam);
    mChorus->setMix(mMixParam);
    mChorus->process(mInputBuffer.get(), mOutputBuffer.get(), mNumSamples);
  } catch (Exception& ex) {
    FAIL();
  }
  GTestUtil::compare(mOutputBuffer.get(), mGroundBuffer.get(), mNumSamples);
}
