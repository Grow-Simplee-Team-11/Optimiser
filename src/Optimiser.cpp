#include "../include/Optimiser.hpp"

Optimizer::Optimizer(RoutePlanInterface* routePlannerInterface_, ClusteringInterface* clusteringInterface_, BinPackInterface* binPackInterface_, vector<item>& packages_, Coordinate& warehouse_, int numberRiders_, Bin& bin_, string logFileName_, bool verbose_, bool logToFile_) {
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
    int avg = 0;
    int maximum = -INT_MAX;
    int maximum = -INT_MAX;
    int avg = 0;
    for(auto& cluster: clusters){
        
        if(verbose){
            cout<<"Printing information for cluster - "<<i<<endl;
        }
        if(logToFile){
            std::ofstream out(logFileName, std::ios_base::app);
            out<<"Printing information for cluster - "<<i<<endl;
        }
        i++;
        avg += cluster.size();
        maximum = max(maximum, (int)cluster.size());
        // Planning routes
        for(int i =0;i<cluster.size();i++){
            cout<<cluster[i].coordinate.latitude<<" "<<cluster[i].coordinate.longitude<<endl;
        }
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
            binPackInterface->PrintPackedDataToFile(logFileName);
        }
        avg+= cluster.size();
        maximum = max(maximum, (int)cluster.size());
    }
    ofstream output;
    output.open("./tests/clusters.txt");
    output << clusterPaths.size() << "\n";
    i=0;
    for(auto& elt: clusterPaths)
    {
        output << i++ << "\n";
        output << elt.size() +2<< "\n";
        output << warehouse.latitude <<" "<< warehouse.longitude<<endl;
        for(auto& it: elt)
        {
            output << it.coordinate.latitude << " " << it.coordinate.longitude << "\n";
        }
        output << warehouse.latitude <<" "<< warehouse.longitude<<endl;
    }
    output.close();
    cout<<"Metrics\n"<<avg/clusters.size()<<"\n"<<maximum<<"\n"<<clusters.size()<<"\n";
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
