#pragma once

#include "PluginProcessor.h"
#include "MyLookAndFeel.h"

//==============================================================================
class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:

    enum Dimen_t {
        paramControlWidth = 250,
        paramControlHeight = 250,
        paramLabelHeight = 50
    };

    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

    explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~AudioPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

private:

    MyLookAndFeel mMyLookAndFeel;
    juce::AudioProcessorValueTreeState& mValueTreeState;

    AudioPluginAudioProcessor& processorRef;

    juce::Slider mDepthSlider;
    std::unique_ptr<SliderAttachment> mDepthSliderAttachment;

    juce::Slider mSpeedSlider;
    std::unique_ptr<SliderAttachment> mSpeedSliderAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};