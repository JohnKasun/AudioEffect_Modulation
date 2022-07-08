#include "Wavetable.h"

Error_t Wavetable::generateSine(float* buffer, int size, float amplitude) {
    if (!buffer)
        return Error_t::kMemError;
    if (size < 0)
        return Error_t::kFunctionInvalidArgsError;

    for (auto i = 0; i < size; i++) {
        buffer[i] = amplitude * sinf(2.0f * M_PI / size * i);
    }
    return Error_t::kNoError;
}

Error_t Wavetable::generateTri(float* buffer, int size, float amplitude) {
    if (!buffer)
        return Error_t::kMemError;
    if (size < 0)
        return Error_t::kFunctionInvalidArgsError;

    auto numRampUp = int{ size / 4 };
    auto numRampDown = int{ size / 2 };
    auto rampDownIdx = int{ numRampUp };
    auto rampUpIdx = int{ rampDownIdx + numRampDown };
    for (auto i = 0; i < rampDownIdx; i++) {
        buffer[i] = amplitude / numRampUp * static_cast<float>(i);
    }
    for (auto i = rampDownIdx; i < rampUpIdx; i++) {
        buffer[i] = -2.0f * amplitude / numRampDown * static_cast<float>(i) + 2.0f * amplitude;
    }
    for (auto i = rampUpIdx; i < size; i++) {
        buffer[i] = amplitude / numRampUp * static_cast<float>(i)  - 4.0f * amplitude;
    }
    return Error_t::kNoError;
}