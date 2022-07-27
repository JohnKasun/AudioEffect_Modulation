#include "APFilter.h"
#include "Synthesis.h"
#include "GTestUtil.h"
#include "DataFileIO.h"

#include <memory>
#include <stdio.h>
#include <math.h>
#include <gtest/gtest.h>

class APFilterTestSuite : public ::testing::Test {
 protected:
  void SetUp() override {
    mNumSamples = 100;
    mInputBuffer.reset(new float[mNumSamples]{});
    mOutputBuffer.reset(new float[mNumSamples]{});
    mGroundBuffer.reset(new float[mNumSamples]{});
  }
  void TearDown() override {}
  std::unique_ptr<float[]> mInputBuffer;
  std::unique_ptr<float[]> mOutputBuffer;
  std::unique_ptr<float[]> mGroundBuffer;
  int mNumSamples;
  APFilter mFilter;
};

TEST_F(APFilterTestSuite, DC) {
  mFilter.setGain(20);
  CSynthesis::generateSine(mInputBuffer.get(), 440, 44100, mNumSamples);
  for (auto i = 0; i < mNumSamples; i++) {
    mOutputBuffer.get()[i] = mFilter.process(mInputBuffer.get()[i]);
  }
  GTestUtil::compare(mOutputBuffer.get(), mGroundBuffer.get(), mNumSamples);
}

TEST_F(APFilterTestSuite, PhaseShift) { 
  float a = 0;
  float f = 11025;
  float fs = 44100;

  float phaseShift =
      -2.0f * M_PI * f / fs + 2 * atan((a * sin(2.0f * M_PI * f / fs) / (1 + a * cos(2.0f * M_PI * f / fs))));
  CSynthesis::generateSine(mInputBuffer.get(), f, fs, mNumSamples);
  CSynthesis::generateSine(mGroundBuffer.get(), f, fs, mNumSamples, 1, phaseShift);
  mFilter.setGain(a);
  for (auto i = 0; i < mNumSamples; i++) {
    mOutputBuffer.get()[i] = mFilter.process(mInputBuffer.get()[i]);
  }
  GTestUtil::compare(mOutputBuffer.get(), mGroundBuffer.get(), mNumSamples);
}
