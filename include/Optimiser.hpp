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
        Optimizer(RoutePlanInterface* routePlannerInterface_, ClusteringInterface* clusteringInterface_, BinPackInterface* binPackInterface_, vector<item>& packages_, Coordinate& warehouse_, int numberRiders_, Bin& bin_, string& logFileName_, bool verbose_, bool logToFile_) {
            routePlannerInterface = routePlannerInterface_;
            clusteringInterface = clusteringInterface_;
            binPackInterface = binPackInterface_;
            packages = packages_;
            warehouse = warehouse_;
            bin = bin_;
            numberRiders = numberRiders_;
            logFileName = logFileName_;
            verbose = verbose_;
            logToFile = logToFile_;
        }

        // Function to perform optimization for set of packages
        void Optimize(){
            clusteringInterface->ComputeClusters(packages, warehouse, numberRiders, bin, 100);
            clusteringInterface->CalculateCost();
            clusteringCost = clusteringInterface->GetClusteringCost();
            clusters = clusteringInterface->GetClusters();

            if(verbose){
                clusteringInterface->PrintClusters();
            }
            if(logToFile){
                clusteringInterface->PrintClustersToFile(logFileName);
            }

            int i = 0;

            for(auto& cluster: clusters){
                
                if(verbose){
                    cout<<"Printing information for cluster - "<<i<<endl;
                }
                if(logToFile){
                    std::ofstream out(logFileName);
                    out<<"Printing information for cluster - "<<i<<endl;
                }
                i++;

                // Planning routes
                routePlannerInterface->PlanRoute(cluster, warehouse, bin);
                clusterPaths.push_back(routePlannerInterface->GetPaths());
                routePlannerInterface->CalculateCost();
                routePlanningCost.push_back(routePlannerInterface->GetPathPlanningCost());

                if(verbose){
                    routePlannerInterface->PrintRoutes();
                }
                if(logToFile){
                    routePlannerInterface->PrintRoutesToFile(logFileName);
                }

                // Computing bin packaging
                binPackInterface->BinPack(cluster, bin);
                clusterPackagings.push_back(binPackInterface->GetPackaging());
                binPackInterface->CalculateCost();
                packagingCost.push_back(binPackInterface->CalculateCost());

                if(verbose){
                    binPackInterface->PrintPackedData();
                }
                if(logToFile){
                    binPackInterface->PrintClustersToFile(logFileName);
                }
            }
            return;
        }

        vector<vector<item>> GetClusters(){
            return clusters;
        }

        vector<item> GetPathForCluster(int seqNumberOfCluster){
            return clusterPaths[seqNumberOfCluster];
        }

        vector<item> GetPackagingForCluster(int seqNumberOfCluster){
            return clusterPackagings[seqNumberOfCluster];
        }

        float GetClusteringCost(){
            return clusteringCost;
        }

        vector<float> GetPackagingCost(){
            return packagingCost;
        }

        vector<float> GetRoutingCost(){
            return routePlanningCost;
        }

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