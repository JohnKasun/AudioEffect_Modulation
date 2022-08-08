#pragma once

#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "AudioFileIf.h"
#include "ErrorDef.h"
#include "Exception.h"
#include "Chorus.h"

using std::cout;
using std::endl;
using std::string;
using std::unique_ptr;
using std::vector;

int main(int argc, char* argv[]) {
  static const int fileBlockSize = 1023;
  const int entryLabelWidth = 20;

  CAudioFileIf* audioFileIn = nullptr;
  CAudioFileIf* audioFileOut = nullptr;
  CAudioFileIf::FileSpec_t fileSpec;

  float** inputAudioData = nullptr;
  float** outputAudioData = nullptr;
  std::vector<unique_ptr<Chorus>> chorus;

  string inputFilePath{};
  string outputFilePath{};

  cout << "\n---- Chorus Effect ----" << endl;

  try {
    // Open Input File
    cout << std::setw(entryLabelWidth) << std::right << "Input WAV file: ";
    std::cin >> inputFilePath;
    CAudioFileIf::create(audioFileIn);
    audioFileIn->openFile(inputFilePath, CAudioFileIf::FileIoType_t::kFileRead);
    if (!audioFileIn->isOpen()) {
      throw Exception("Couldn't open input file...");
    }
    audioFileIn->getFileSpec(fileSpec);

    // Open Output File
    outputFilePath = inputFilePath;
    for (int i = 0; i < 4; i++) {
      outputFilePath.pop_back();
    }
    outputFilePath.append("Out.wav");
    CAudioFileIf::create(audioFileOut);
    audioFileOut->openFile(outputFilePath, CAudioFileIf::FileIoType_t::kFileWrite, &fileSpec);
    if (!audioFileOut->isOpen()) {
      throw Exception("Couldn't open output file...");
    }

    // Create and initialize instance
    for (int c = 0; c < fileSpec.iNumChannels; c++) {
      chorus.emplace_back(new Chorus(fileSpec.fSampleRateInHz));
      chorus.back()->setDepth(10);
      chorus.back()->setSpeed(0.25);
      chorus.back()->setNumVoices(3);
      chorus.back()->setShape(Chorus::Shape::Sine);
    }

    // Allocate memory
    inputAudioData = new float* [fileSpec.iNumChannels] {};
    outputAudioData = new float* [fileSpec.iNumChannels] {};
    for (int c = 0; c < fileSpec.iNumChannels; c++) {
      inputAudioData[c] = new float[fileBlockSize]{};
      outputAudioData[c] = new float[fileBlockSize]{};
    }

    // Process
    long long iNumFrames = fileBlockSize;
    while (!audioFileIn->isEof()) {
      if (audioFileIn->readData(inputAudioData, iNumFrames) != Error_t::kNoError)
        throw Exception("Data reading error...");
      for (int c = 0; c < fileSpec.iNumChannels; c++) {
        chorus[c]->process(inputAudioData[c], outputAudioData[c], iNumFrames);
      }
      if (audioFileOut->writeData(outputAudioData, iNumFrames) != Error_t::kNoError)
        throw Exception("Data writing error...");
    }

    // Clean-up
    for (int c = 0; c < fileSpec.iNumChannels; c++) {
      delete[] inputAudioData[c];
      delete[] outputAudioData[c];
    }
    delete[] inputAudioData;
    delete[] outputAudioData;

    CAudioFileIf::destroy(audioFileIn);
    CAudioFileIf::destroy(audioFileOut);
    chorus.clear();

    cout << endl;
    cout << std::setw(entryLabelWidth) << "Done processing..." << endl;
    cout << "\n---- Chorus Effect ---- " << endl;
    cout << endl;
    return 0;
  } catch (Exception& ex) {
    if (audioFileIn) {
      CAudioFileIf::destroy(audioFileIn);
      audioFileIn = nullptr;
    }
    if (audioFileOut) {
      CAudioFileIf::destroy(audioFileOut);
      audioFileOut = nullptr;
    }
    if (inputAudioData) {
      for (int c = 0; c < fileSpec.iNumChannels; c++) {
        delete[] inputAudioData[c];
      }
      delete[] inputAudioData;
      inputAudioData = nullptr;
    }
    if (outputAudioData) {
      for (int c = 0; c < fileSpec.iNumChannels; c++) {
        delete[] outputAudioData[c];
      }
      delete[] outputAudioData;
      outputAudioData = nullptr;
    }
    cout << "\n--------------------------" << endl;
    cout << ex.what() << endl;
    cout << "--------------------------" << endl;
    return -1;
  }
}