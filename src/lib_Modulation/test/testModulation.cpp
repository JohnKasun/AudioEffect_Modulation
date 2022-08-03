#pragma once

#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "ErrorDef.h"
#include "GTestUtil.h"
#include "ModulationIf.h"
#include "Synthesis.h"
#include "Vector.h"

class ModulationTestSuite : public ::testing::Test {
 protected:
  virtual void SetUp() override {
    mSampleRate = float{44100};
    mNumSamples = int{10000};
    mMod = std::make_unique<ModulationIf>();
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
  std::unique_ptr<ModulationIf> mMod;
  std::unique_ptr<float[]> mInputBuffer;
  std::unique_ptr<float[]> mOutputBuffer;
  std::unique_ptr<float[]> mGroundBuffer;
};

class FlangerTestSuite : public ModulationTestSuite {
 protected:
  void SetUp() override {
    ModulationTestSuite::SetUp();
    mMod->init(mSampleRate, ModulationIf::Type::Flanger);
  }
};

TEST_F(FlangerTestSuite, ZeroDepth) {
  mSpeedParam = 1;
  CSynthesis::generateSine(mInputBuffer.get(), 440, mSampleRate, mNumSamples);
  auto delayInSamp = int{CUtil::float2int<int>(ModulationIf::getMaxDepth() * mSampleRate / 1000) / 2};
  CVectorFloat::copy(mGroundBuffer.get(), mInputBuffer.get(), mNumSamples);
  CVectorFloat::add_I(mGroundBuffer.get() + delayInSamp, mInputBuffer.get(), mNumSamples - delayInSamp);
  CVectorFloat::mulC_I(mGroundBuffer.get(), 0.5f, mNumSamples);
  mMod->setDepth(mDepthParam);
  mMod->setSpeed(mSpeedParam);
  mMod->process(mInputBuffer.get(), mOutputBuffer.get(), mNumSamples);
  GTestUtil::compare(mOutputBuffer.get(), mGroundBuffer.get(), mNumSamples);
}

TEST_F(FlangerTestSuite, ZeroSpeed) {
  mDepthParam = 20.0f;
  CSynthesis::generateSine(mInputBuffer.get(), 440, mSampleRate, mNumSamples);
  CVectorFloat::copy(mGroundBuffer.get(), mInputBuffer.get(), mNumSamples);
  CVectorFloat::add_I(mGroundBuffer.get(), mInputBuffer.get(), mNumSamples);
  CVectorFloat::mulC_I(mGroundBuffer.get(), 0.5f, mNumSamples);
  mMod->setDepth(mDepthParam);
  mMod->setSpeed(mSpeedParam);
  mMod->process(mInputBuffer.get(), mOutputBuffer.get(), mNumSamples);
  GTestUtil::compare(mOutputBuffer.get(), mGroundBuffer.get(), mNumSamples);
}

