//
//  WaveForm.cpp
//  Wavolution
//
//  Created by Ryan Larson on 12/26/16.
//  Copyright © 2016 RyanLarson. All rights reserved.
//

#pragma once

#include <iostream>
#include <vector>
using namespace std;

class WaveForm
{
public:
    unsigned int    numChannels;
    unsigned int    sampleRate;
    unsigned int    bitsPerSample;
    double          fundamentalFrequency; // this doesn't necissarily belong here. it's just convenient for inverse FT.
    size_t          numSamples;
    vector<double>  data; // samples
    
    WaveForm(
              unsigned int numChannels = 1,
              unsigned int sampleRate = 96000,
              unsigned int bitsPerSample = 32,
              double       fundamentalFrequency = 46.875, // F#0 +24 cents  (serum recomended)
              size_t       numSamples = 2048
              )
    {
        this->numChannels = numChannels;
        this->sampleRate = sampleRate;
        this->bitsPerSample = bitsPerSample;
        this->fundamentalFrequency = fundamentalFrequency;
        this->numSamples = numSamples;
    }
    
    void reset()
    {
        WaveForm();
        data.clear();
    }
};
