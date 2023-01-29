/**
 * Ensembler.hpp
 * ~Rohit Raj
**/
#ifndef __ENSEMBLER__
#define __ENSEMBLER__

#include<string>
#include "interface/RoutePlanInterface.hpp"
#include "interface/BinPackInterface.hpp"
#include "interface/ClusteringInterface.hpp"
#include "Optimiser.hpp"
#include <pthread.h>

using namespace std;

void* run(void *);

class Ensembler{
    public:
        Ensembler(vector<string>&, vector<string>&, vector<string>&, vector<BinPackInterface*>&, vector<ClusteringInterface*>&, vector<RoutePlanInterface*>&, vector<item>& packages, Coordinate& warehouse, int numberRiders, Bin& bin);
        void Report();
        void EnsembleRun();
        vector<pair<vector<string>,double>> GetCosts();
    
        vector<string> RoutePlanningAlgorithms;
        vector<string> BinPackingAlgorithms;
        vector<string> ClusteringAlgorithms;

        vector<RoutePlanInterface*> RoutePlanningInterfaces;
        vector<BinPackInterface*> BinPackingInterfaces;
        vector<ClusteringInterface*> ClusteringInterfaces;

        vector<Ensembler*> ensemblersUsed;

        vector<item> packages;
        Coordinate warehouse;
        int numberRiders;
        Bin bin;

        int* currentCombination;
        
        vector<pair<vector<string>, double>> Costs;
};

#endif 