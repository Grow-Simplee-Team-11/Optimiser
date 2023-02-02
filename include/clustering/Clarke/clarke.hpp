// #include "datastructures.hpp"
// #include "../../include/datastructures.hpp"
#ifndef __CLARKE__HEADER__
#define __CLARKE__HEADER__
#include "../../../include/interface/ClusteringInterface.hpp"
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
        int p1,p2;// left and right enpoints of cluster
        double weight;// total weight of the cluster
        double volume;// total volume of the cluster
        double distance;// total path length of cluster
        int rank;// rank of the cluster DSU
    Cluster();
};

class Clarke: public ClusteringInterface{
    public:
    Clarke(DistanceType method) : ClusteringInterface(method){}// constructor
    vector<item> packages; Coordinate warehouse; int numPackages;int numRiders; Bin b;double distThresh;
    vector<Cluster> Clusters;// clusters formed from clarke wright algorithm
    vector<Cluster> final_Clusters;// final clusters after further consolidation
    vector<int> parent;// parent array for DSU
    priority_queue<pair<double,pair<int,int>>> q;// priority queue for savings
    double depotDist(Coordinate&);// distance from warehouse to package
    // void Clarke::setParams(vector<item> packages, Coordinate warehouse, int numRiders, Bin b);
    void solve();
    public:
    void ComputeClusters(vector<item>& packages, Coordinate warehouse, int numRiders, Bin b);// main function --> calls all other functions and returns the final clusters
    double compute_savings(item&,item&);// compute savings for a pair of packages
    void create_pq();// create priority queue for savings
    void make_set(int);// make set for DSU
    int find_set(int);// find set for DSU
    void test();// test function
    void consolodate_further();// consolidates clusters further until the number of clusters is equal to the number of riders
    bool cmp(const pair<int,int> ,const pair<int,int> );// comparator for sorting the clusters' likeliness to be merged
    void merge_sort(vector<pair<int,int>> , int  , int );// merge sort for sorting the clusters' likeliness to be merged
    Cluster initCluster(int);// initialize a cluster
    int union_sets(int a,int b, bool);// union sets for DSU
    void CalculateCost();// calculate cost of each cluster
};
#endif