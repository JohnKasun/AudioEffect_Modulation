#include "Lfo.h"

Lfo::Lfo(float sampleRate){

	for (int param = 0; param < numParams; param++) {
		mParamValues[param] = 0.0f;
	}
	for (auto& buffer : mBuffers) {
		buffer.reset(new CRingBuffer<float>(mBufferSize));
	}

	mSampleRate = sampleRate;

	// Sine
	float temp[mBufferSize]{};
	Wavetable::generateSine(temp, mBufferSize);
	mBuffers.at(Waveform_t::Sine)->putPostInc(temp, mBufferSize);

	// Triangle
	Wavetable::generateTri(temp, mBufferSize);
	mBuffers.at(Waveform_t::Triangle)->putPostInc(temp, mBufferSize);
}

void Lfo::setParam(Param_t param, float value){
	if (param == Param_t::phaseInRadians) {
		for (auto& buffer : mBuffers) {
			buffer->setReadIdx(CUtil::float2int<int>(value / (2.0f * M_PI) * buffer->getLength()));
		}
	}
	mParamValues[param] = value;
}

void Lfo::setWaveform(Waveform_t waveform)
{
	mCurrentWaveform = waveform;
}

float Lfo::getParam(Param_t param) const{
	return mParamValues[param];
}

Lfo::Waveform_t Lfo::getWaveform() const
{
	return mCurrentWaveform;
}

float Lfo::process(){
	float currentValue = mBuffers.at(mCurrentWaveform)->get(mCurrentIndex);
	mCurrentIndex += mParamValues[freqInHz] / mSampleRate * mBufferSize;
	return mParamValues[dc] + mParamValues[amplitude] * currentValue;
}
