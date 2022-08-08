#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudioPluginAudioProcessor::AudioPluginAudioProcessor()
    : AudioProcessor(BusesProperties()
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      mParameters(*this, nullptr, juce::Identifier("Parameters"), {
        std::make_unique<juce::AudioParameterFloat>("depth", "Depth", 0.0f, Chorus::MaxDepthInMs, 10.0f),
            std::make_unique<juce::AudioParameterFloat>("speed", "Speed", 0.0f, Chorus::MaxSpeedInHz, 0.25f),
            std::make_unique<juce::AudioParameterInt>("voices", "Number Of Voices", 1, Chorus::MaxNumVoices, 3),
            std::make_unique<juce::AudioParameterChoice>("waveform", "Waveform", juce::StringArray{"Sine", "Triangle"}, 0),
            std::make_unique<juce::AudioParameterFloat>("gain", "Gain", 0.0f, 5.0f, 1.0f),
            std::make_unique<juce::AudioParameterFloat>("mix", "Mix", 0.0f, 1.0f, 0.5f)
        })
{
    mDepthParam = mParameters.getRawParameterValue("depth");
    mSpeedParam = mParameters.getRawParameterValue("speed");
    mVoicesParam = mParameters.getRawParameterValue("voices");
    mWaveformParam = mParameters.getRawParameterValue("waveform");
    mGainParam = mParameters.getRawParameterValue("gain");
    mMixParam = mParameters.getRawParameterValue("mix");
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor()
{
}

//==============================================================================
const juce::String AudioPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const
{
    return false;
}

bool AudioPluginAudioProcessor::producesMidi() const
{
    return false;
}

bool AudioPluginAudioProcessor::isMidiEffect() const
{
    return false;
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String AudioPluginAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void AudioPluginAudioProcessor::changeProgramName(int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void AudioPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    for (auto& chorus : mChorus) {
        chorus.reset(new Chorus(sampleRate));
    }
}

void AudioPluginAudioProcessor::releaseResources()
{
    for (auto& chorus : mChorus) {
        chorus.reset();
    }
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
    juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;

    for (auto& chorus : mChorus) {
      chorus->setDepth(*mDepthParam);
      chorus->setSpeed(*mSpeedParam);
      chorus->setNumVoices(static_cast<int>(*mVoicesParam));
      Chorus::Shape waveform;
      switch (static_cast<int>(*mWaveformParam)) { 
      case 0:
        waveform = Chorus::Shape::Sine;
        break;
      default:
        waveform = Chorus::Shape::Triangle;
      }
      chorus->setShape(waveform);
      chorus->setGain(*mGainParam);
      chorus->setMix(*mMixParam);
    }
    
    auto inputBuffer = getBusBuffer(buffer, true, 0);
    auto outputBuffer = getBusBuffer(buffer, false, 0);
    for (auto c = 0; c < inputBuffer.getNumChannels(); c++) {
        mChorus[c]->process(buffer.getReadPointer(c), buffer.getWritePointer(c), buffer.getNumSamples());
    }
    for (auto c = inputBuffer.getNumChannels(); c < outputBuffer.getNumChannels(); c++) {
        buffer.copyFrom(c, 0, buffer.getReadPointer(0), buffer.getNumSamples());
    }
}

//==============================================================================
bool AudioPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor()
{
    return new AudioPluginAudioProcessorEditor(*this, mParameters);
}

//==============================================================================
void AudioPluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = mParameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr) {
        if (xmlState->hasTagName(mParameters.state.getType())) {
            mParameters.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudioPluginAudioProcessor();
}