/**
 * ClusteringInterface.hpp 
 * ~ Archisman Pathak
**/

#ifndef __CLUSTERING_INTERFACE__
#define __CLUSTERING_INTERFACE__

#include<vector>
#include<cstdint>
#include<string>
#include<iostream>
#include<fstream>

#include "../datastructures.hpp"

using namespace std;

class ClusteringInterface{
    public:
        ClusteringInterface() {
            cost = 0;
        }
        // store the computed cluster in clusters 
        virtual void ComputeClusters(vector<item> packages, Coordinate warehouse, int numRiders, Bin b) = 0;
        // calculate the cost of the cluster computed 
        virtual float CalculateCost() = 0;

        void PrintClusters()
        {
            int32_t i=0;
            for(auto &cluster : clusters)
            {
                cout<<"Printing Cluster -- " << i++;
                for(auto &it : cluster){
                    // TODO : Complete Me
                }
            }
        }

        void PrintClustersToFile(string filename)
        {
            std::ofstream out(filename);
            int32_t i=0;
            for(auto &cluster : clusters)
            {
                out<<"Printing Cluster -- " << i++;
                for(auto &it : cluster){
                    // TODO : Complete Me
                }
            }
        }
    private:
        vector<vector<item>> clusters;
        float cost;

};
#endif 