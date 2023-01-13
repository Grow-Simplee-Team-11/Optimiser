/**
 * Optimizer.hpp
 * ~Rohit Raj
**/
#ifndef __OPTIMIZER__
#define __OPTIMIZER__

#include<string>
#include "interface/RoutePlanInterface.hpp"
#include "interface/BinPackInterface.hpp"
#include "interface/ClusteringInterface.hpp"

using namespace std;

class Optimizer{
    public:
        Optimizer(RoutePlanInterface* routePlannerInterface_, ClusteringInterface* clusteringInterface_, BinPackInterface* binPackInterface_, vector<item>& packages_, Coordinate& warehouse_, int numberRiders_, Bin& bin_, string& logFileName_, bool verbose_, bool logToFile_);

        // Function to perform optimization for set of packages
        void optimize();
        vector<vector<item>> GetClusters();

        vector<item> GetPathForCluster(int seqNumberOfCluster);

        vector<item> GetPackagingForCluster(int seqNumberOfCluster);
        float GetClusteringCost();

        vector<float> GetPackagingCost();

        vector<float> GetRoutingCost();

    private:
        RoutePlanInterface* routePlannerInterface;
        ClusteringInterface* clusteringInterface;
        BinPackInterface* binPackInterface;
        vector<item> packages;
        Coordinate warehouse;
        int numberRiders;
        Bin bin;
        std::string logFileName;
        bool verbose;
        bool logToFile;

        vector<vector<item>> clusters;
        vector<vector<item>> clusterPaths;
        vector<vector<item>> clusterPackagings;

        // Cost of each segment
        float clusteringCost;
        vector<float> packagingCost;
        vector<float> routePlanningCost;
};

#endif 