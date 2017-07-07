//
//  WaveFileReader.cpp
//  Wavolution
//
//  Created by Ryan Larson on 12/26/16.
//  Copyright © 2016 RyanLarson. All rights reserved.
//

#pragma once

#include <fstream>
#include <array>
#include "WaveForm.cpp"
using namespace std;

#include "EndianIO.cpp"
using namespace endianIO;


class WaveFileReader
{
    ifstream f;
    
 
public:
    
    // gets a big endian string of a specified length (not including delimiter) from a binary stream
    string getStringFromBinaryStream(ifstream &f, size_t charLength)
    {
        charLength += 1;
        char tmp[5];
        f.get(tmp, 5);
        return string(tmp);
    }
    
    
    void parseWave(string fileName, WaveForm &wf)
    {
        f.open(fileName, ios::binary);
        
        wf.reset();
        
        
        // === RIFF chunk descriptor ===
        string chunkID;
        unsigned int chunkSize = 0;
        string format;
        
        // ====== fmt sub-chunk ======
        string subChunk1ID;
        unsigned int subChunk1Size;
        unsigned int audioFormat = 0;
        unsigned int numChannels = 0;
        unsigned int sampleRate = 0;
        unsigned int bitsPerSample = 0;
        
        // === data subchunk ===
        unsigned int subchunk2Size;
        vector<double> data;

        
        // read each chunk name
        // if it matches a known type: read the data
        // else: skip chunk
        string chunkName;
        
        while(chunkName = getStringFromBinaryStream(f, 4), !f.eof())
        {
            if(chunkName == "RIFF")
            {
                read_little(f, chunkSize, 4);
                format = getStringFromBinaryStream(f, 4);
            }
            else if(chunkName == "fmt ")
            {
                read_little(f, subChunk1Size, 4);
                read_little(f, audioFormat, 2);
                read_little(f, numChannels, 2);
                read_little(f, sampleRate, 4);
                f.ignore(4);//byte rate
                f.ignore(2);//block align
                read_little(f, bitsPerSample, 2);
            }
            else if(chunkName == "data")
            {
                read_little(f, subchunk2Size, 4);
                wf.numSamples = subchunk2Size / (bitsPerSample / 8);
                
                if(audioFormat == 1)
                { // integer PCM
                    for(size_t i = 0; i<wf.numSamples; ++i)
                    {
                        int value;
                        read_little(f, value, bitsPerSample/8);
                        data.push_back(double(value));
                    }
                }else if(audioFormat == 3 && bitsPerSample == 32)
                { // (assumed to be) 32 bit float PCM
                    
                    for(size_t i = 0; i<wf.numSamples; ++i)
                    {
                        int32_t value;
                        read_little(f, value, bitsPerSample/8);
                        data.push_back(double(*reinterpret_cast<float*>(&value)));
                    }
                }
            }
            else
            {
                size_t chunkSize;
                read_little(f, chunkSize, 4);
                f.ignore(chunkSize);
            }
        }
        
        wf.numChannels = numChannels;
        wf.sampleRate = sampleRate;
        wf.bitsPerSample = bitsPerSample;
        wf.data = data;
    }  
};
















