//
//  WaveFileMaker.cpp
//  Wavolution
//
//  Created by Ryan Larson on 12/23/16.
//  Copyright © 2016 RyanLarson. All rights reserved.
//

#pragma once

#include <fstream>
#include <cmath>
#include <array>
#include "WaveForm.cpp"
using namespace std;

#include "EndianIO.cpp"
using namespace endianIO;

// creates a WAVE file consisting of a single cycle waveform from a list of harmonic multiple amplitudes
class WaveFileMaker
{
    ofstream f;
    
    // === RIFF chunk descriptor ===
    string RIFF_chunkDescriptor;
    
    
    // === fmt sub-chunk ===
    unsigned int subChunk1Size;
    unsigned int audioFormat;
    unsigned int byteRate;
    unsigned int blockAlign;
    
    
    // === data sub-chunk ===
    string                      subChunk2ID;
    size_t                      subChunk2Size;
    const double                twoPI;
    
    
public:
    
    WaveFileMaker() :
        twoPI(2 * M_PI)
    {
        // === RIFF chunk descriptor ===
        RIFF_chunkDescriptor = "RIFF____WAVEfmt ";
        
        
        // === fmt sub-chunk ===
        subChunk1Size  = 16; // 16 for PCM (audio format 1)
        audioFormat    = 1; // PCM
        
        
        // === data sub-chunk ===
        subChunk2ID             = "data";
    }
    
    
    
    // creates a wave file from a single waveform
    void makeWaveFile(WaveForm wf,  string fileName = "wavolution.wav")
    {
        // === calculate remaining needed values ===
        
        // --- fmt sub-chunk ---
        byteRate       = wf.sampleRate * wf.numChannels * wf.bitsPerSample / 8;
        blockAlign     = wf.numChannels * wf.bitsPerSample / 8;
        
        // --- data sub-chunk ---
        size_t numSamples = wf.data.size();
        subChunk2Size  = numSamples * wf.numChannels * wf.bitsPerSample / 8;
        
        
        
        // === write to file ===
        
        f.open(fileName, ios::binary);
        
        // --- RIFF chunk descriptor ---
        f << RIFF_chunkDescriptor;
        
        // --- fmt sub-chunk ---
        write_little(f,   subChunk1Size,    4);
        write_little(f,   audioFormat,      2);
        write_little(f,   wf.numChannels,   2);
        write_little(f,   wf.sampleRate,    4);
        write_little(f,   byteRate,         4);
        write_little(f,   blockAlign,       2);
        write_little(f,   wf.bitsPerSample, 2);
        
        // --- data sub-chunk ---
        f << subChunk2ID;
        write_little(f, subChunk2Size, 4);
        
        for( size_t i = 0; i < numSamples; ++i)
        {
            write_little(f, int32_t(wf.data[i]), 4);
        }
        
        f.close();
    }
    
    
};





































