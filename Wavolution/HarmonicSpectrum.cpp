//
//  HarmonicSpectrum.cpp
//  Wavolution
//
//  Created by Ryan Larson on 1/3/17.
//  Copyright © 2017 RyanLarson. All rights reserved.
//

#pragma once

#include <vector>
#include <cstdlib>
#include <time.h>
using namespace std;


class HarmonicSpectrum
{
public:
    const int maxAmplitude = 1000000000;
    vector<int> data; // ordered list of harmonic overtone amplitudes, where the first element is the fundamental frequency
    
    HarmonicSpectrum(int size = 128)
    {
        data.resize(size);
    }
    
    HarmonicSpectrum(vector<double> &harmonicAmplitude)
    {
        data.resize(harmonicAmplitude.size());
        for(size_t i=0; i<data.size(); ++i)
            data[i] = harmonicAmplitude[i];
    }
    
    void operator=(HarmonicSpectrum const &hs)
    {
        data = hs.data;
    }
    
    void fillWithRandomData(int seed)
    {
        // fill data with random values in range 0 - maxAmplitude
        srand(seed);
        for(int i=0; i<data.size(); ++i)
            data[i] = (rand() * rand()) % maxAmplitude;
    }
};
