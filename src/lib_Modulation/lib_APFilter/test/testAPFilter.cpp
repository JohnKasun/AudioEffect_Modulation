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
    mSampleRate = 48000;
    mInputBuffer.reset(new float[mNumSamples]{});
    mOutputBuffer.reset(new float[mNumSamples]{});
    mGroundBuffer.reset(new float[mNumSamples]{});
    mFilter.reset(new APFilter(mSampleRate));
  }
  void TearDown() override {}
  std::unique_ptr<float[]> mInputBuffer;
  std::unique_ptr<float[]> mOutputBuffer;
  std::unique_ptr<float[]> mGroundBuffer;
  int mNumSamples;
  float mSampleRate;
  std::unique_ptr<APFilter> mFilter;

  float calculateA(float fb, float fs) { return (tanf(M_PI * fb / fs) - 1) / (tanf(M_PI * fb / fs) + 1);}
  float calculatePhaseShift(float f, float a, float fs) {
    return -2.0f * M_PI * f / fs + 2 * atan((a * sin(2.0f * M_PI * f / fs) / (1 + a * cos(2.0f * M_PI * f / fs))));
  }
};

TEST_F(APFilterTestSuite, BreakFrequency) { 
  float f = 6000;
  float fb = 6000;
  EXPECT_EQ(fb, f);
      
  CSynthesis::generateSine(mInputBuffer.get(), f, mSampleRate, mNumSamples); 
  CSynthesis::generateSine(mGroundBuffer.get(), f, mSampleRate, mNumSamples, 1, -1.0 * M_PI / 2);  // phase shift by -pi/2
  mFilter->setParam(APFilter::BreakFreq, fb);
  for (auto i = 0; i < mNumSamples; i++) {
    mOutputBuffer.get()[i] = mFilter->process(mInputBuffer.get()[i]);
  }
  auto latency = mFilter->getLatency();
  EXPECT_TRUE(latency < mNumSamples);
  GTestUtil::compare(mGroundBuffer.get() + latency, mOutputBuffer.get() + latency, mNumSamples - latency, 1E-3);
}

TEST_F(APFilterTestSuite, DC) {
  float f = 0;
  float fb = 10000;

  CSynthesis::generateDc(mInputBuffer.get(), mNumSamples);
  mFilter->setParam(APFilter::BreakFreq, fb);
  for (auto i = 0; i < mNumSamples; i++) {
    mOutputBuffer.get()[i] = mFilter->process(mInputBuffer.get()[i]);
  }
  auto latency = mFilter->getLatency();
  EXPECT_TRUE(latency < mNumSamples);
  GTestUtil::compare(mInputBuffer.get() + latency, mOutputBuffer.get() + latency, mNumSamples - latency, 1E-3);
}

TEST_F(APFilterTestSuite, Nyquist) {
  float f = 49.999;
  float fb = 25;

  mSampleRate = 100;
  mFilter.reset(new APFilter(mSampleRate));
  CSynthesis::generateSine(mInputBuffer.get(), f, mSampleRate, mNumSamples);
  CSynthesis::generateSine(mGroundBuffer.get(), f, mSampleRate, mNumSamples, 1, -1.0 * M_PI);  // phase shift by -pi
  mFilter->setParam(APFilter::BreakFreq, fb);
  for (auto i = 0; i < mNumSamples; i++) {
    mOutputBuffer.get()[i] = mFilter->process(mInputBuffer.get()[i]);
  }
  auto latency = mFilter->getLatency();
  EXPECT_TRUE(latency < mNumSamples);
  GTestUtil::compare(mGroundBuffer.get() + latency, mOutputBuffer.get() + latency, mNumSamples - latency, 1E-3);
}


