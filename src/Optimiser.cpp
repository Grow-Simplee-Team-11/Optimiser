#include "../include/Optimiser.hpp"

Optimizer::Optimizer(RoutePlanInterface* routePlannerInterface_, ClusteringInterface* clusteringInterface_, BinPackInterface* binPackInterface_, vector<item>& packages_, Coordinate& warehouse_, int numberRiders_, Bin& bin_, string& logFileName_, bool verbose_, bool logToFile_) {
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

void Optimizer::optimize(){
    clusteringInterface->ComputeClusters(packages, warehouse, numberRiders, bin);
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
        routePlannerInterface->PlanRoute(cluster, warehouse);
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

vector<vector<item>> Optimizer::GetClusters(){
    return clusters;
}

vector<item> Optimizer::GetPathForCluster(int seqNumberOfCluster){
    return clusterPaths[seqNumberOfCluster];
}

vector<item> Optimizer::GetPackagingForCluster(int seqNumberOfCluster){
    return clusterPackagings[seqNumberOfCluster];
}

float Optimizer::GetClusteringCost(){
    return clusteringCost;
}

vector<float> Optimizer::GetPackagingCost(){
    return packagingCost;
}

vector<float> Optimizer::GetRoutingCost(){
    return routePlanningCost;
}
