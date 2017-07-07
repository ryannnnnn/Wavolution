//
//  main.cpp
//  Wavolution
//
//  Created by Ryan Larson on 12/23/16.
//  Copyright © 2016 RyanLarson. All rights reserved.
//

#include <iostream>
#include <cmath>
#include "WaveFileMaker.cpp"
#include "WaveFileReader.cpp"
#include "WaveForm.cpp"
#include "FourierTransform.cpp"
#include "GeneticAlgorithm.cpp"
using namespace std;

#include "TestWaves.cpp"
using namespace testWaves;






int main(int argc, const char * argv[]) {
    
    string wavOut = "/Users/ryan/Desktop/wavOut.wav";
    string wavIn = "/Users/ryan/Desktop/wavIn.wav";

    
    
//    { // manual shapes
//        
//        WaveForm wf;
//        
//        FourierTransform ft;
//        vector<double> ha = squareWave(32);
//        HarmonicSpectrum hs(ha);
//        ft.inverseTransform(hs, wf);
//        
//        
//        WaveFileMaker wfm;
//        wfm.makeWaveFile(wf, wavOut);
//        
//    }
    
    
    
    
//    {// translate wav
//
//        WaveForm wf;
//        
//        WaveFileReader wr;
//        wr.parseWave(wavIn, wf);
//        
//        WaveFileMaker wfm;
//        wfm.makeWaveFile(wf, wavOut);
//        
//    }
    
    
    
//    { // figure out serum "compression"
//        
//        WaveForm wf;
//        
//        for(size_t i=0; i<=wf.numSamples; ++i)
//            wf.data.push_back(i);
//        
//        WaveFileMaker wfm;
//        wfm.makeWaveFile(wf, wavOut);
//        
//    }
    
    
//    {  // test GE
//        
//        GeneticEvaluator ge1, ge2;
//        FourierTransform ft;
//        
//        WaveForm rule;
//        vector<double> ha = squareWave(32);
//        HarmonicSpectrum hs(ha);
//        ft.inverseTransform(hs, rule);
//        
//        HarmonicSpectrum h2;
//        h2.fillWithRandomData(42);
//        
//        ge1.analyzeRule(rule);
//        ge2.analyzeCandidate(h2);
//        cout << ge1.similarity(ge2) << endl;
//    }
    
    
    
    
//    {// GA from manual shape
//        
//        FourierTransform ft;
//        
//        WaveForm rule;
//        vector<double> ha = squareWave(32);
////        vector<double> ha = sineWave();
//        HarmonicSpectrum hs(ha);
//        ft.inverseTransform(hs, rule);
//        
//        GeneticAlgorithm ga;
//        HarmonicSpectrum result = ga.performTournament(rule);
//        
//        WaveForm wf;
//        
//        
//        ft.inverseTransform(result, wf);
//        
//        WaveFileMaker wfm;
//        wfm.makeWaveFile(wf, wavOut);
//        
//    }
    
    
    
//    {// GA from wav file
//        
//        FourierTransform ft;
//        
//        WaveForm wf;
//        WaveFileReader wr;
//        wr.parseWave(wavIn, wf);
//
//        
//        GeneticAlgorithm ga;
//        HarmonicSpectrum result = ga.performTournament(wf);
//        
//        
//        WaveForm wf_result;
//        
//        ft.inverseTransform(result, wf_result);
//        
//        WaveFileMaker wfm;
//        wfm.makeWaveFile(wf_result, wavOut);
//        
//    }
    
    
//    { // forward fourier transform manual shape *** removing phase from input waveform ***
//
//        vector<double> manualShape_ha = squareWave(128);
////        vector<double> manualShape_ha = sineWave();
//        HarmonicSpectrum manualShape_hs(manualShape_ha);
//        
//        FourierTransform ft;
//        WaveForm manualShape_wf;
//        ft.inverseTransform(manualShape_hs, manualShape_wf);
//        
//        HarmonicSpectrum forwardResult;
//        ft.forward(manualShape_wf, forwardResult);
//        
//        WaveForm dephased_wf;
//        ft.inverseTransform(forwardResult, dephased_wf);
//        
//        WaveFileMaker wfm;
//        wfm.makeWaveFile(dephased_wf, wavOut);
//    }
    
    
    { // wav file -> dephase -> GA

        FourierTransform ft;
        
        WaveForm input_wf;
        WaveFileReader wr;
        wr.parseWave(wavIn, input_wf);
        
        HarmonicSpectrum input_hs;
        ft.forward(input_wf, input_hs);
        
        WaveForm dephased_wf;
        ft.inverseTransform(input_hs, dephased_wf);
        
        GeneticAlgorithm ga;
        HarmonicSpectrum result_hs = ga.performTournament(dephased_wf);
        
        WaveForm result_wf;
        ft.inverseTransform(result_hs, result_wf);
        
        WaveFileMaker wfm;
        wfm.makeWaveFile(result_wf, wavOut);
    }
    

    return 0;
}





















