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

#include "clustering/fesif/fesif.hpp"
#include "clustering/selfClustering/selfClustering.hpp"
#include "clustering/Clarke/clarke.hpp"
#include "clustering/HGS/HGS.hpp"

#include "routeplan/TSP_OR.hpp"
#include "routeplan/TSP_LK.hpp"
#include "routeplan/TSP_CK.hpp"
#include "routeplan/TSP_OR_EDD.hpp"

#include "binpack/EB_AFIT.hpp"

using namespace std;

void* run(void *);


class Ensembler{
    public:
        Ensembler(vector<string>&, vector<string>&, vector<string>&, vector<item>& packages, Coordinate& warehouse, int numberRiders, Bin& bin);
        void Report();
        void EnsembleRun();
        vector<pair<vector<string>,double>> GetCosts();
    
        vector<string> RoutePlanningAlgorithms;
        vector<string> BinPackingAlgorithms;
        vector<string> ClusteringAlgorithms;

        vector<Ensembler*> ensemblersUsed;

        vector<item> packages;
        Coordinate warehouse;
        int numberRiders;
        Bin bin;

        int* currentCombination;
        
        vector<pair<vector<string>, double>> Costs;
};

#endif 