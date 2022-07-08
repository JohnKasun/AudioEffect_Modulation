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

    auto numRampUp = int{ size / 2 };
    for (auto i = 0; i < numRampUp; i++) {
        buffer[i] = amplitude * static_cast<float>(i) / numRampUp;
    }
    for (auto i = numRampUp; i < size; i++) {
        buffer[i] = amplitude * (2.0f - (static_cast<float>(i) / numRampUp));
    }
    return Error_t::kNoError;
}