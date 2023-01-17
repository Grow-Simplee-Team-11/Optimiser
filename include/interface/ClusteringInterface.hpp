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
        virtual void ComputeClusters(vector<item>& packages, Coordinate warehouse, int numRiders, Bin b) = 0;
        // calculate the cost of the cluster computed 
        virtual void CalculateCost() = 0;

        void PrintClusters()
        {
            int32_t i=0;
            for(auto &cluster : clusters)
            {
                cout<<"Printing Cluster -- " << i++ <<endl;
                for(auto &it : cluster){
                    it.print();
                }
                
            }
            return;
        }

        void PrintClustersToFile(string filename)
        {
            std::ofstream out(filename, std::ios_base::app);
            int32_t i=0;
            for(auto &cluster : clusters)
            {
                out<<"Printing Cluster -- " << i++ <<" \n";
                for(auto &it : cluster){
                    it.printToFile(out);
                }
                
            }
            return;
        }

        vector<vector<item>> GetClusters(){
            return clusters;
        }

        float GetClusteringCost(){
            return cost;
        }

        void setClusters(vector<vector<item> > createdClusters) {
            clusters = createdClusters;
        }

        Coordinate getWarehouse(){
            return warehouse;
        }

        
    protected:
        vector<vector<item>> clusters;
        float cost;
        Coordinate warehouse;
        Bin b;
        int numRiders;

};
#endif 