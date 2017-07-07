//
//  GeneticEvaluator.cpp
//  Wavolution
//
//  Created by Ryan Larson on 1/3/17.
//  Copyright © 2017 RyanLarson. All rights reserved.
//

#pragma once

#include <cmath>
#include <vector>
#include "HarmonicSpectrum.cpp"
#include "FourierTransform.cpp"
using namespace std;


// analyzes a waveform to create a set of numerical "rules" with which to judge similiarity to other waveforms
class GeneticEvaluator
{
    WaveForm wf;
    
    // rule measurments
    vector<double> avgRunning; // running average of amplitude, parallel to number of samples
//    double maxAmp;
//    double minAmp;
    double avgAmpChange; // average change bewteen two samples
    //double maxAmpChange;
    //double minAmpChange;
    //size_t degree; // number of times the waveform changes direction + 1
    
    
    // rule weights
    
    
    // generate ruleset from candidate waveform that will be compaired with other waveforms
    void analyze()
    {
//        // min and max amp
//        minAmp = wf.data[0];
//        maxAmp = wf.data[0];
//        
//        for(size_t i=0; i<wf.data.size(); ++i)
//        {
//            if(wf.data[i] < minAmp) minAmp = wf.data[i];
//            else if (wf.data[i] > maxAmp) maxAmp = wf.data[i];
//        }
        
        
        
        // avgRunning
        avgRunning.resize(wf.numSamples);
        double runningTotal = 0;
        for(int i=0; i<wf.numSamples; ++i)
        {
            runningTotal += wf.data[i];
            avgRunning[i] = runningTotal / (i+1);
        }
        
        
        // average change between samples
        avgAmpChange = 0;
        for(int i=1; i<wf.numSamples; ++i)
            avgAmpChange += abs(wf.data[i] - wf.data[i-1]);
        avgAmpChange /= wf.numSamples;
    }
    
    
    
    double normCorr_avgRunning(const GeneticEvaluator &ge)
    {
        double normCorr_running; // normalized correlation -1...1
        
        double numerator_running = 0;
        for(size_t i=0; i<avgRunning.size(); ++i) numerator_running += avgRunning[i]*ge.avgRunning[i];
        
        double x_running = 0;
        double y_running = 0;
        for(size_t i=0; i<avgRunning.size(); ++i)
        {
            x_running += pow(avgRunning[i], 2);
            y_running += pow(ge.avgRunning[i], 2);
        }
        
        normCorr_running = numerator_running / sqrt(x_running * y_running);
        
        // adjust range from -1...1 to 0...1
        normCorr_running = (normCorr_running - -1) / (1 - -1);
        
        return normCorr_running;
    }
    
    
    
    // doesn't do shit,  always 1.   probably because i'm trying to correlate vectors of size 1
    double normCorr_avgChange(const GeneticEvaluator &ge)
    {
        double normCorr_avgChange;
        
        double numerator_avgChange = avgAmpChange * ge.avgAmpChange;
        double x_avgChange = pow(avgAmpChange, 2);
        double y_avgChange = pow(ge.avgAmpChange, 2);
        
        normCorr_avgChange = numerator_avgChange / sqrt(x_avgChange * y_avgChange);
        
        // adjust range from -1...1 to 0...1
        normCorr_avgChange = (normCorr_avgChange - -1) / (1 - -1);
        
        return normCorr_avgChange;
    }
    
    
//    vector<double> normalizeWaveform(const vector<double> wf)
//    {
//        
//    }
    
    
    double normCorr_waveform(const GeneticEvaluator &ge)
    {
        
        // the waveform
        double normCorr; // normalized correlation -1...1
        
        double numerator = 0;
        for(size_t i=0; i<wf.data.size(); ++i) numerator += wf.data[i]*ge.wf.data[i];
        
        double x = 0;
        double y = 0;
        for(size_t i=0; i<wf.data.size(); ++i)
        {
            x += pow(wf.data[i], 2);
            y += pow(ge.wf.data[i], 2);
        }
        
        normCorr = numerator / sqrt(x*y);
        
        
        // adjust range from -1...1 to 0...2
        normCorr += 1;
        
        // adjust range from 0...2 to 0...1
        return (normCorr - 0) / (2 - 0);
    }
    

    
public:
    
    GeneticEvaluator() :
        avgRunning(0)
    {
        
    }
    
    
    void analyzeCandidate(const HarmonicSpectrum c)
    {
        FourierTransform().inverseTransform(c, wf);
        analyze();
    }
    
    void analyzeRule(WaveForm wf)
    {
        this->wf = wf;
        analyze();
    }
    

    
    // returns a number 0-1 (percentage) that represents how similar the argument's waveform is to this instance's waveform (via rule compairison)
    // 1 being as similar as possible, and 0 as dissimilar as possible
    double similarity(const GeneticEvaluator &ge)
    {
        
        
        
        
//        double similarity = 0;
//        similarity += normCorr_avgRunning(ge) * 1; // multiplied by beta values
//        similarity += normCorr_waveform(ge) * 1; // multiplied by beta values
//        
//        auto a = (similarity - 0) / (2 - 0);
//        
//        
//        return (similarity - 0) / (2 - 0); // normalize results added together to range 0...1
        
        return normCorr_waveform(ge);
    }
    
    
    

};























