//
//  TestWaves.cpp
//  Wavolution
//
//  Created by Ryan Larson on 1/3/17.
//  Copyright © 2017 RyanLarson. All rights reserved.
//

#pragma once

#include "HarmonicSpectrum.cpp"


namespace testWaves
{
    unsigned int maxAmplitude = 1000000000;
    
    vector<double> squareWave(size_t size)
    {
        vector<double> harmonic_amplitude(size, 0);
        for (int i=0; i<size; ++i)
        {
            if (i % 2 == 0) //odd frequencies
                harmonic_amplitude[i] = maxAmplitude / (i*1.1+1);
        }
        return harmonic_amplitude;
    }
    
    vector<double> cinemaWave(size_t size)
    {
        vector<double> harmonic_amplitude;
        for (int i=0; i<size; ++i)
        {
            if (i % 2 == 0) //odd frequencies
                for(int k=0; k<size; k++)
                {
                    harmonic_amplitude.push_back(maxAmplitude * ((maxAmplitude - i*2) / maxAmplitude - i));
                }
        }
        return harmonic_amplitude;
    }
    
    
    
    vector<double> sineWave()
    {
        vector<double> harmonic_amplitude;
        harmonic_amplitude.push_back(maxAmplitude);
        return harmonic_amplitude;
    }
    
    
    vector<double> reverseSawWave(size_t size)
    {
        vector<double> harmonic_amplitude(size);
        for (int i=0; i<size; ++i)
        {
            harmonic_amplitude[i] = maxAmplitude/(i+1);
        }
        return harmonic_amplitude;
    }
}
