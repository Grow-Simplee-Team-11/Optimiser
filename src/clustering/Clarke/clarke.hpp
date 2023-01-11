#include "datastructures.hpp"
#include "ClusteringInterface.hpp"
#include<iostream>
#include<cmath>
#include<vector>
#include<map>
#include<queue>
using namespace std;


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
    void ComputeClusters(vector<item> , Coordinate , int , Bin ,double );
    double compute_savings(item&,item&);
    void create_pq();
    void make_set(int);
    int find_set(int);
    Cluster initCluster(int);
    void union_sets(int a,int b);
    void solve();
    double CalculateCost();
};