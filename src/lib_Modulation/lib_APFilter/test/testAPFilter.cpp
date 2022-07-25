#include "APFilter.h"
#include "Synthesis.h"
#include "GTestUtil.h"

#include <memory>
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
  mFilter.setGain(1);
  CSynthesis::generateDc(mInputBuffer.get(), mNumSamples);
  for (auto i = 0; i < mNumSamples; i++) {
    mOutputBuffer.get()[i] = mFilter.process(mInputBuffer.get()[i]);
  }
  GTestUtil::compare(mOutputBuffer.get(), mGroundBuffer.get(), mNumSamples);
}
