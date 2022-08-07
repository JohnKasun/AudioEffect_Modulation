#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor(&p), processorRef(p), mValueTreeState(vts)
{
    juce::ignoreUnused(processorRef);

    addAndMakeVisible(mDepthSlider);
    mDepthSliderAttachment.reset(new SliderAttachment(mValueTreeState, "depth", mDepthSlider));
    mDepthSlider.setName("Depth");
    mDepthSlider.setSliderStyle(juce::Slider::Rotary);
    mDepthSlider.setLookAndFeel(&mMyLookAndFeel);
    mDepthSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, paramControlWidth, paramLabelHeight);

    addAndMakeVisible(mSpeedSlider);
    mSpeedSliderAttachment.reset(new SliderAttachment(mValueTreeState, "speed", mSpeedSlider));
    mSpeedSlider.setName("Speed");
    mSpeedSlider.setSliderStyle(juce::Slider::Rotary);
    mSpeedSlider.setLookAndFeel(&mMyLookAndFeel);
    mSpeedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, paramControlWidth, paramLabelHeight);

    addAndMakeVisible(mVoicesSlider);
    mVoicesSliderAttachment.reset(new SliderAttachment(mValueTreeState, "voices", mVoicesSlider));
    mVoicesSlider.setName("Voices");
    mVoicesSlider.setSliderStyle(juce::Slider::Rotary);
    mVoicesSlider.setLookAndFeel(&mMyLookAndFeel);
    mVoicesSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, paramControlWidth, paramLabelHeight);

    addAndMakeVisible(mWaveformSlider);
    mWaveformSliderAttachment.reset(new SliderAttachment(mValueTreeState, "waveform", mWaveformSlider));
    mWaveformSlider.setName("Waveform");
    mWaveformSlider.setSliderStyle(juce::Slider::Rotary);
    mWaveformSlider.setLookAndFeel(&mMyLookAndFeel);
    mWaveformSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, paramControlWidth, paramLabelHeight);

    setSize(paramControlWidth * 2, (paramControlHeight + paramLabelHeight) * 2);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor()
{
}

//==============================================================================
void AudioPluginAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void AudioPluginAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    auto leftArea = area.removeFromLeft(paramControlWidth);
    auto topLeftArea = leftArea.removeFromTop(paramControlHeight + paramLabelHeight);
    auto topRightArea = area.removeFromTop(paramControlHeight + paramLabelHeight);

    mDepthSlider.setBounds(topLeftArea);
    mSpeedSlider.setBounds(topRightArea);
    mVoicesSlider.setBounds(leftArea);
    mWaveformSlider.setBounds(area);
}