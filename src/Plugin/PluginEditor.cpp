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

    addAndMakeVisible(mDelaySlider);
    mDelaySliderAttachment.reset(new SliderAttachment(mValueTreeState, "delay", mDelaySlider));
    mDelaySlider.setName("Delay");
    mDelaySlider.setSliderStyle(juce::Slider::Rotary);
    mDelaySlider.setLookAndFeel(&mMyLookAndFeel);
    mDelaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, paramControlWidth, paramLabelHeight);

    setSize(paramControlWidth * 3, paramControlHeight + paramLabelHeight);
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
    auto DepthArea = area.removeFromLeft(paramControlWidth);
    auto SpeedArea = area.removeFromLeft(paramControlWidth);
    auto DelayArea = area;

    mDepthSlider.setBounds(DepthArea);
    mSpeedSlider.setBounds(SpeedArea);
    mDelaySlider.setBounds(DelayArea);
}