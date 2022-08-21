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

    addAndMakeVisible(mWaveformSelector);
    mWaveformSelector.addItemList({"Sine", "Triangle"}, 0);
    mWaveformSelectorAttachment.reset(new ComboBoxAttachment(mValueTreeState, "waveform", mWaveformSelector));
    mWaveformSelector.setLookAndFeel(&mMyLookAndFeel);
    mWaveformSelector.setSelectedItemIndex(1, juce::sendNotification);

    addAndMakeVisible(mGainSlider);
    mGainSliderAttachment.reset(new SliderAttachment(mValueTreeState, "gain", mGainSlider));
    mGainSlider.setName("Gain");
    mGainSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mGainSlider.setLookAndFeel(&mMyLookAndFeel);
    mGainSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, paramLinearSliderLabelWidth, paramLabelHeight);

    addAndMakeVisible(mMixSlider);
    mMixSliderAttachment.reset(new SliderAttachment(mValueTreeState, "mix", mMixSlider));
    mMixSlider.setName("Mix");
    mMixSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    mMixSlider.setLookAndFeel(&mMyLookAndFeel);
    mMixSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, paramLinearSliderLabelWidth, paramLabelHeight);

    setSize(paramControlWidth * 2, (paramControlHeight + paramLabelHeight + paramLinearSliderHeight) * 2);
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
    auto footer = area.removeFromBottom(paramLinearSliderHeight * 2);
    auto leftArea = area.removeFromLeft(paramControlWidth);
    auto topLeftArea = leftArea.removeFromTop(paramControlHeight + paramLabelHeight);
    auto topRightArea = area.removeFromTop(paramControlHeight + paramLabelHeight);

    mDepthSlider.setBounds(topLeftArea);
    mSpeedSlider.setBounds(topRightArea);
    mVoicesSlider.setBounds(leftArea);
    mWaveformSelector.setBounds(area);
    mGainSlider.setBounds(footer.removeFromTop(paramLinearSliderHeight));
    mMixSlider.setBounds(footer);
}