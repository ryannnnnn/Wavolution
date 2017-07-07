//
//  GeneticAlgorithm.cpp
//  Wavolution
//
//  Created by Ryan Larson on 1/3/17.
//  Copyright © 2017 RyanLarson. All rights reserved.
//

#pragma once

// ** cout for testing ***
#include <iostream>
//***********************

#include <vector>
#include <algorithm>
#include <cstdlib>
#include <time.h>
#include <future>
#include "HarmonicSpectrum.cpp"
#include "GeneticEvaluator.cpp"
#include "GeneManipulator.cpp"
using namespace std;




class Candidate
{
public:
    HarmonicSpectrum data;
    GeneticEvaluator ge;
    double fitness;
    
    void analyze()
    {
        ge.analyzeCandidate(data);
    }
    
    bool operator> (Candidate b)
    {
        return fitness > b.fitness;
    }
};


// GA using tournament selection
class GeneticAlgorithm
{
    const int populationSize;
    const int tournamentSize;
    const int maxNumberOfTournaments;
    const double solutionThreshold; // minimum fitness percentage required to qualify as soultion
    
    vector<Candidate> candidates;
    
    GeneticEvaluator rule; // this is the genetic evaluation of the waveform(s) used to compare with the candidates' genetic evaluations
    GeneManipulator gm;
    

    void createInitialPopulation()
    {
        for(size_t i = 0; i<populationSize; ++i)
        {
            candidates[i].data.fillWithRandomData(rand());
            candidates[i].analyze();
            candidates[i].fitness = rule.similarity(candidates[i].ge);
        }
    }
    
    
    
    // performs the tournament on one set of participants (to be called async)
    // returns true if it generated a candidate that meeets the fitness solution threshold, false otherwise
    // participants vector includes more elements than needed (the others are used by seperate calls to this function). the usefull range is specified by firstParticipant...lastParticipant, which are indicies for the participants vector
    bool tournament(vector<Candidate> &candidates, const double &solutionThreshold, const vector<int> &participants, int firstParticipant, int lastParticipant)
    {
        // find two best candidates, and worst candidate
        size_t max0, max1, min; // max0 : first best fitness    max1: seccond best fitness
        
        // max0 and max1
        if(candidates[participants[firstParticipant]].fitness > candidates[participants[firstParticipant+1]].fitness)
        {
            max0 = firstParticipant;
            max1 = firstParticipant+1;
        }else
        {
            max0 = firstParticipant+1;
            max1 = firstParticipant;
        }
        
        // min
        min = (candidates[participants[firstParticipant]].fitness < candidates[participants[firstParticipant+1]].fitness) ? firstParticipant : firstParticipant+1;
        
        
        // iterate the remaing candidates
        for( int i = firstParticipant; i <= lastParticipant; ++i)
        {
            if(candidates[participants[i]].fitness > candidates[participants[max0]].fitness)
            {
                max1 = max0;
                max0 = i;
            }
            else if( candidates[participants[i]].fitness > candidates[participants[max1]].fitness)
                max1 = i;
            else if( candidates[participants[i]].fitness < candidates[participants[min]].fitness )
                min = i;
        }
        
        
        // crossover two best candidates to create new child
        HarmonicSpectrum child = gm.uniformCrossOver(candidates[participants[max0]].data, candidates[participants[max1]].data);
        //            gm.twoPointCrossover(candidates[max0].data, candidates[b].data);
        
        
        //mutate child
        gm.mutate(child, rand());
        
        
        
        // replace worst worst candidate with new child
        candidates[participants[min]].data = child;
        candidates[participants[min]].analyze();
        
        
        // re-evaluate the new candidate, and check if it's a solution
        candidates[participants[min]].fitness = rule.similarity(candidates[participants[min]].ge);
        if(candidates[participants[min]].fitness >= solutionThreshold)
            return true;
        return false;
    }
    
    
    
    // manages dividing up the poulation, and dispatching async tournaments
    // "round" refers to the set of async dispatches for one division of the population into tournamets
    void runTournament()
    {
        vector<int> candidatesIndex(populationSize);
        bool solutionFound = false; // TODO: not being used  lol.  could just set round = numrounds
        

        // populate the parallel array of population indicies, then shuffle it
        for(int i=0; i<candidatesIndex.size(); ++i)
            candidatesIndex[i] = i;
        random_shuffle(candidatesIndex.begin(), candidatesIndex.end());
        
        
        
        const int numTournamentsPerRound = ( populationSize / tournamentSize ); // the last tournament will include any remaining candidates that did not divide evenly.
        const int numRounds = maxNumberOfTournaments / numTournamentsPerRound;
        for(int round=0; round<numRounds; ++round)
        {
            // divide shuffled population into tournament sized chunks (the last tournmanet will include any remaining candidates that did not divide evenly)
            //run tournaments in async
            vector<future<bool>> tournamentHandler(numTournamentsPerRound); // each tournament will return a bool representing if it generated a solution
            for( int i=0; i<numTournamentsPerRound; ++i)
            {
                int firstParticipant = tournamentSize * i;
                int lastPartcipant = (i == numTournamentsPerRound-1) ? int(candidatesIndex.size())-1 : firstParticipant + tournamentSize - 1;
                auto lambda = [&]{ return tournament(candidates, solutionThreshold, candidatesIndex, firstParticipant, lastPartcipant); };
                tournamentHandler[i] = async(launch::async, lambda);
            }
 
            
            // wait for results. check if solution was found
            for(int i=0; i<numTournamentsPerRound; ++i)
            {
                if(tournamentHandler[i].get() == true)
                {
                    solutionFound = true;
                    round = numRounds;
                }
            }
            
            
            
            // *** temp stuff ***********************************************
            // display percentage completed
            static int complete = 0;
            if((float(round)/numRounds) * 100 > complete)
                cout << ++complete << endl;
            // **************************************************************
            
            
        }
    }
    
    
    
    Candidate& findBestCandidate()
    {
        size_t max = 0;
        
        for( int i = 0; i<candidates.size(); ++i)
            if(candidates[i].fitness > candidates[max].fitness)
                max = i;
        return candidates[max];
    }
    
    
public:
    
    GeneticAlgorithm() :
        populationSize(600),
        tournamentSize(50),
        maxNumberOfTournaments(200000),
        solutionThreshold(0.98),
        candidates(populationSize)
    {
        srand(int(time(NULL)));
    }
    
    
    HarmonicSpectrum performTournament(WaveForm wfRule)
    {
        rule.analyzeRule(wfRule);
        createInitialPopulation();
        runTournament();
        Candidate &result = findBestCandidate();
        cout << result.fitness << endl;
        return result.data;
    }
    
};






















