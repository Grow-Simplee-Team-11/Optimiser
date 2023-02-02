#ifndef SELF_CLUSTERING_H
#define SELF_CLUSTERING_H
#include <bits/stdc++.h>
#include "../../interface/ClusteringInterface.hpp"
#include "../../datastructures.hpp"

/*******************************************/
/* HEADER FILE FOR PETAL SWEEP ALGORITHM   */
/*******************************************/
using namespace std;

class SELFCLUSTERING : public ClusteringInterface {

    private:
        vector<vector<item>> calculateCluster(vector<item>& items, Coordinate warehouse, int numberOfRiders, Bin bin);// main function --> executes the Petal Sweep Algorithm returns the final clusters

    public:
        const static int angularCutMetric = 3;
        SELFCLUSTERING(DistanceType method) : ClusteringInterface(method){// constructor for SELF CLUSTERING Class
            // delta = rho;
        }
        void ComputeClusters(vector<item>& packages, Coordinate warehouse, int numRiders, Bin b) {// main function --> calls all other functions and returns the final clusters
            setClusters(calculateCluster(packages, warehouse, numRiders, b));
        }
        void CalculateCost(){
            cost = 0;
        }
};

#endif  