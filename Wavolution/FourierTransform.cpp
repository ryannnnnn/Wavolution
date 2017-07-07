//
//  FourierTransform.cpp
//  Wavolution
//
//  Created by Ryan Larson on 12/29/16.
//  Copyright © 2016 RyanLarson. All rights reserved.
//



/*
 
 converts between waveform and frequency band
 
 forward: (waveform -> harmonic spectrum)
 inverse: (harmonic spectrum -> waveform)
 
 
 */

#pragma once

#include <cmath>
#include <vector>
#include "WaveForm.cpp"
#include "HarmonicSpectrum.cpp"
using namespace std;

class FourierTransform
{
    const double twoPI = 2 * M_PI;

public:

    // converts a harmonic spectrum to a waveform
    // the provided WaveForm must be populated with everything but the wave's sample data
    void inverseTransform(const HarmonicSpectrum &harmonicSpectrum, WaveForm &wf)
    {
        
        // --- calculate the frequency of each harmonic ---
        // * the first harmonic is the fundmental frequency *
        
        size_t size = harmonicSpectrum.data.size();
        vector<double> harmonic_frequency(size);
        for(int i=0; i<size; ++i)
        {
            harmonic_frequency[i] = wf.fundamentalFrequency * (i+1);
        }
        
        
        
        // --- calculate waveform samples ---
        wf.data.resize(wf.numSamples);
        for( int i = 0; i < wf.numSamples; ++i)
        {
            double sample = 0;
            for( int j = 0; j < harmonicSpectrum.data.size(); ++j)
            {
                sample += harmonicSpectrum.data[j] * sin( (twoPI * i * harmonic_frequency[j]) / wf.sampleRate );
            }
            wf.data[i] = sample;
        }
    }
    
    
    // translated from: https://gist.github.com/anonymous/129d477ddb1c8025c9ac
    void forward(const WaveForm &wf, HarmonicSpectrum &hs)
    {
        long hs_origSize = hs.data.size();
        hs.data.resize(wf.data.size());
        
        long N = wf.data.size();
        double realPart = 0; // running total
        double imaginaryPart = 0; // running total
        double rate;
        double time;
        double distance;
        
        for(int freq = 1; freq < N; ++freq)
        {
            realPart = 0;
            imaginaryPart = 0;
            
            for(int t = 0; t < N; ++t)
            {
                rate = -(2*M_PI) * freq;
                time = double(t) / N;
                distance = rate * time;
                realPart += wf.data[t] * cos(distance);
                imaginaryPart += wf.data[t] * sin(distance);
            }
            
            if(abs(realPart) < 1e-10) realPart = 0;
            if(abs(imaginaryPart) < 1e-10) imaginaryPart = 0;
            
            realPart /= N;
            imaginaryPart /= N;
            
            hs.data[freq-1] = sqrt(realPart*realPart + imaginaryPart*imaginaryPart);
        }
        
        hs.data.resize(hs_origSize);
        
        //TODO: probbaly need to normalize the result to range 0...maxHarmonicAmplitude
    }
};




















