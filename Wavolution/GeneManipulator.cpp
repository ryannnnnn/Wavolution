//
//  GeneManipulator.cpp
//  Wavolution
//
//  Created by Ryan Larson on 1/3/17.
//  Copyright © 2017 RyanLarson. All rights reserved.
//

#pragma once

#include "HarmonicSpectrum.cpp"
#include <cstdlib>
#include <time.h>
using namespace std;

class GeneManipulator
{

    double probabilityOfMutation = 0.01; // *** if you want more precision (less than 1%), you will need to edit mutate() ***
  
public:
    
    GeneManipulator()
    {
        srand(int(time(NULL)));
    }
    
    void swap(HarmonicSpectrum &a, HarmonicSpectrum &b, size_t first, size_t last = 0)
    {
        if(last < first) last = first;
        
        int tmp;
        for(size_t i=first; i<=last; ++i)
        {
            tmp = a.data[i];
            a.data[i] = b.data[i];
            b.data[i] = tmp;
        }
        
    }
    
    // randomly select two points, and swap the region contained by them
    void twoPointCrossover(HarmonicSpectrum &a, HarmonicSpectrum &b)
    {
        size_t p1 = rand() % a.data.size(),
               p2 = rand() % a.data.size();
        
        if( p1 > p2) swap(a, b, p2, p1);
            else swap(a, b, p1, p2);
    }
    
    
    // for each overtone in the child, a 50% chance decides from which parent the value comes
    HarmonicSpectrum uniformCrossOver(const HarmonicSpectrum &a, const HarmonicSpectrum &b)
    {
        HarmonicSpectrum child;
        for(int i=0; i<a.data.size(); ++i)
            child.data[i] = (rand() % 2 == 0) ? a.data[i] : b.data[i];
        return child;
    }
    
    // with probability, repalces a random index's value in the data with a random value
    void mutate(HarmonicSpectrum &hs, int randSeed)
    {
        srand(randSeed);
        if( ((rand()%100)/100) <= probabilityOfMutation )
            hs.data[rand()%hs.data.size()] = rand()%hs.maxAmplitude;
    }
    
    
//    // each pair of overtone values are swapped on a 50% chance
//    void uniformCrossOver(HarmonicSpectrum &a, HarmonicSpectrum &b)
//    {
//        for(int i=0; i<a.data.size(); ++i)
//            if(rand() % 2 == 0)
//                swap(a, b, i);
//    }
  
};
