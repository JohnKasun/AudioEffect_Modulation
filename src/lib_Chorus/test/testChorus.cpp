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
    mChorus.reset(new Chorus(mSampleRate, 20.0f, 3));
    mInputBuffer.reset(new float[mNumSamples]{});
    mOutputBuffer.reset(new float[mNumSamples]{});
    mGroundBuffer.reset(new float[mNumSamples]{});
    mDepthParam = float{0};
    mSpeedParam = float{0};
  }
  float mSampleRate;
  int mNumSamples;
  float mDepthParam;
  float mSpeedParam;
  std::unique_ptr<Chorus> mChorus;
  std::unique_ptr<float[]> mInputBuffer;
  std::unique_ptr<float[]> mOutputBuffer;
  std::unique_ptr<float[]> mGroundBuffer;
};
TEST_F(ChorusTestSuite, ZeroDepth) {
  mSpeedParam = 1;
  CSynthesis::generateSine(mInputBuffer.get(), 440, mSampleRate, mNumSamples);
  auto delayInSamp = int{CUtil::float2int<int>(20.0f * mSampleRate / 1000) / 2};
  CVectorFloat::copy(mGroundBuffer.get(), mInputBuffer.get(), mNumSamples);
  CVectorFloat::add_I(mGroundBuffer.get() + delayInSamp, mInputBuffer.get(), mNumSamples - delayInSamp);
  CVectorFloat::mulC_I(mGroundBuffer.get(), 0.5f, mNumSamples);
  mChorus->setDepth(mDepthParam);
  mChorus->setSpeed(mSpeedParam);
  mChorus->process(mInputBuffer.get(), mOutputBuffer.get(), mNumSamples);
  GTestUtil::compare(mOutputBuffer.get(), mGroundBuffer.get(), mNumSamples);
}

TEST_F(ChorusTestSuite, ZeroSpeed) {
  mDepthParam = 20.0f;
  CSynthesis::generateSine(mInputBuffer.get(), 440, mSampleRate, mNumSamples);
  CVectorFloat::copy(mGroundBuffer.get(), mInputBuffer.get(), mNumSamples);
  CVectorFloat::add_I(mGroundBuffer.get(), mInputBuffer.get(), mNumSamples);
  CVectorFloat::mulC_I(mGroundBuffer.get(), 0.5f, mNumSamples);
  mChorus->setDepth(mDepthParam);
  mChorus->setSpeed(mSpeedParam);
  mChorus->process(mInputBuffer.get(), mOutputBuffer.get(), mNumSamples);
  GTestUtil::compare(mOutputBuffer.get(), mGroundBuffer.get(), mNumSamples);
}
