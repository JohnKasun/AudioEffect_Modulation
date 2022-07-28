#include "APFilter.h"
#include "Synthesis.h"
#include "GTestUtil.h"
#include "DataFileIO.h"
#include "Vector.h"

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

  float calculateA(float fb, float fs) { return (tanf(M_PI * fb / fs) - 1) / (tanf(M_PI * fb / fs) + 1);}
  float calculatePhaseShift(float f, float a, float fs) {
    return -2.0f * M_PI * f / fs + 2 * atan((a * sin(2.0f * M_PI * f / fs) / (1 + a * cos(2.0f * M_PI * f / fs))));
  }
};

TEST_F(APFilterTestSuite, BreakFrequency) { 
  float f = 6000;
  float fb = 6000;
  EXPECT_EQ(fb, f);
  float fs = 48000;
  float a = calculateA(fb, fs);
      
  CSynthesis::generateSine(mInputBuffer.get(), f, fs, mNumSamples); 
  CSynthesis::generateSine(mGroundBuffer.get(), f, fs, mNumSamples, 1, -1.0 * M_PI / 2);  // phase shift by -pi/2
  auto samplesPerCycle = int{CUtil::float2int<int>(fs / f)};
  mFilter.setGain(a);
  for (auto i = 0; i < mNumSamples; i++) {
    mOutputBuffer.get()[i] = mFilter.process(mInputBuffer.get()[i]);
  }
  EXPECT_TRUE(samplesPerCycle < mNumSamples);
  GTestUtil::compare(mGroundBuffer.get(), mOutputBuffer.get() + samplesPerCycle, mNumSamples - samplesPerCycle, 1E-3);
}

TEST_F(APFilterTestSuite, DC) {
  float f = 0;
  float fb = 3000;
  float fs = 48000;
  float a = calculateA(fb, fs);

  CSynthesis::generateDc(mInputBuffer.get(), mNumSamples);
  mFilter.setGain(a);
  for (auto i = 0; i < mNumSamples; i++) {
    mOutputBuffer.get()[i] = mFilter.process(mInputBuffer.get()[i]);
  }
  GTestUtil::compare(mInputBuffer.get(), mOutputBuffer.get(), mNumSamples, 1E-3);
}

