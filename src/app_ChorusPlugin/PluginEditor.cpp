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
    mDepthSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);

    addAndMakeVisible(mSpeedSlider);
    mSpeedSliderAttachment.reset(new SliderAttachment(mValueTreeState, "speed", mSpeedSlider));
    mSpeedSlider.setName("Speed");
    mSpeedSlider.setSliderStyle(juce::Slider::Rotary);
    mSpeedSlider.setLookAndFeel(&mMyLookAndFeel);
    mSpeedSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);

    addAndMakeVisible(mVoicesSlider);
    mVoicesSliderAttachment.reset(new SliderAttachment(mValueTreeState, "voices", mVoicesSlider));
    mVoicesSlider.setName("Voices");
    mVoicesSlider.setSliderStyle(juce::Slider::Rotary);
    mVoicesSlider.setLookAndFeel(&mMyLookAndFeel);
    mVoicesSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);

    addAndMakeVisible(mWaveformSelector);
    mWaveformSelector.addItemList({"Sine", "Triangle"}, 1);
    mWaveformSelectorAttachment.reset(new ComboBoxAttachment(mValueTreeState, "waveform", mWaveformSelector));
    mWaveformSelector.setLookAndFeel(&mMyLookAndFeel);

    addAndMakeVisible(mGainSlider);
    mGainSliderAttachment.reset(new SliderAttachment(mValueTreeState, "gain", mGainSlider));
    mGainSlider.setName("Gain");
    mGainSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mGainSlider.setLookAndFeel(&mMyLookAndFeel);
    mGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(mGainLabel);
    mGainLabel.attachToComponent(&mGainSlider, true);
    mGainLabel.setText("Gain", juce::dontSendNotification);

    addAndMakeVisible(mMixSlider);
    mMixSliderAttachment.reset(new SliderAttachment(mValueTreeState, "mix", mMixSlider));
    mMixSlider.setName("Mix");
    mMixSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mMixSlider.setLookAndFeel(&mMyLookAndFeel);
    mMixSlider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);

    setSize(paramControlWidth * 2, (paramControlHeight + paramLinearSliderHeight) * 2);
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
    auto mixArea = area.removeFromBottom(paramLinearSliderHeight);
    auto gainArea = area.removeFromBottom(paramLinearSliderHeight);
    auto leftArea = area.removeFromLeft(paramControlWidth);
    auto topLeftArea = leftArea.removeFromTop(paramControlHeight);
    auto topRightArea = area.removeFromTop(paramControlHeight);

    mDepthSlider.setBounds(topLeftArea);
    mSpeedSlider.setBounds(topRightArea);
    mVoicesSlider.setBounds(leftArea);
    mWaveformSelector.setBounds(area);

    mGainLabel.setBounds(gainArea.removeFromLeft(paramLinearSliderLabelWidth));
    mGainSlider.setBounds(gainArea);
    mMixSlider.setBounds(mixArea);
}