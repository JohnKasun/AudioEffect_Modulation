#pragma once

#include <vector>

#include "ErrorDef.h"
#include "gtest/gtest.h"
struct GTestUtil {
 public:
  static Error_t compare(float* buff1, float* buff2, int numSamples, float tolerance = 1E-3) {
    if (!buff1 || !buff2 || numSamples < 0)
      return Error_t::kFunctionInvalidArgsError;

    for (int i = 0; i < numSamples; i++) {
      EXPECT_NEAR(buff1[i], buff2[i], tolerance);
    }
    return Error_t::kNoError;
  }
  static Error_t compare(float** buff1, float** buff2, int numChannels, int numSamples, float tolerance = 1E-3) {
    if (numChannels < 0)
      return Error_t::kFunctionInvalidArgsError;

    for (int c = 0; c < numChannels; c++) {
      compare(buff1[c], buff2[c], numSamples, tolerance);
    }
    return Error_t::kNoError;
  }
  static Error_t compare(float* buff1, std::vector<float>& buff2, int numSamples, float tolerance = 1E-3) {
    if (!buff1 || numSamples < 0)
      return Error_t::kFunctionInvalidArgsError;

    for (int i = 0; i < numSamples; i++) {
      EXPECT_NEAR(buff1[i], buff2.at(i), tolerance);
    }
    return Error_t::kNoError;
  }
};