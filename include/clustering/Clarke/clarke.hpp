// #include "datastructures.hpp"
// #include "../../include/datastructures.hpp"
#ifndef __CLARKE__HEADER__
#define __CLARKE__HEADER__
#include "../../interface/ClusteringInterface.hpp"
using namespace std;


#include<iostream>
#include<cmath>
#include<vector>
#include<map>
#include<queue>
using namespace std;
const double R = 6371;

class Cluster{
    public:
        int p1,p2;
        double weight;
        double volume;
        double distance;
        int rank;
    Cluster();
};

class Clarke: public ClusteringInterface{
    public:
    vector<item> packages; Coordinate warehouse; int numPackages;int numRiders; Bin b;double distThresh;
    vector<Cluster> Clusters;
    vector<int> parent;
    priority_queue<pair<double,pair<int,int>>> q;
    double depotDist(Coordinate&);
    // void Clarke::setParams(vector<item> packages, Coordinate warehouse, int numRiders, Bin b);
    void solve();
    public:
    void ComputeClusters(vector<item>& packages, Coordinate warehouse, int numRiders, Bin b);
    double compute_savings(item&,item&);
    void create_pq();
    void make_set(int);
    int find_set(int);
    void test();
    Cluster initCluster(int);
    void union_sets(int a,int b);
    void CalculateCost();
};
#endif