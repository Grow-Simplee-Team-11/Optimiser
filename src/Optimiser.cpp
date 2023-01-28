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
    int i = 0;
    int maximum = -INT_MAX;
    int avg = 0;
    int dropoffs = INT_MAX;
    vector<item> main_packages = packages;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> Temporal_Factor_Gen(clusteringInterface->Temporal_Factor_Lower_Limit, clusteringInterface->Temporal_Factor_Upper_Limit);
    std::uniform_real_distribution<double> Spactial_Factor_Gen(clusteringInterface->Spatial_Factor_Lower_Limit, clusteringInterface->Spatial_Factor_Upper_Limit);
    for(int j = 0 ; j <= 10 ; j++){
        clusteringInterface->Temporal_Factor = Temporal_Factor_Gen(gen);
        clusteringInterface->Spatial_Factor = Spactial_Factor_Gen(gen);
        cout<<"Now starting iteration with Temporal Factor ====> "<<clusteringInterface->Temporal_Factor<<endl;
        cout<<"Now starting iteration with Spatial Factor ====> "<<clusteringInterface->Spatial_Factor<<endl;
        clusteringInterface->ComputeClusters(main_packages, warehouse, numberRiders, bin);
        clusteringInterface->CalculateCost();
        clusters = clusteringInterface->GetClusters();
        
        for(auto& cluster: clusters){
            routePlannerInterface->PlanRoute(cluster, warehouse);
            routePlannerInterface->CalculateCost();
        }
        if(routePlannerInterface->drop_offs < dropoffs){
            cout<<"New Best DropOffs ==> "<<routePlannerInterface->drop_offs<<endl;
            cout<<"With Temporal Factor ===> "<<clusteringInterface->Temporal_Factor<<endl;
            cout<<"With Spatial Factor ===> "<<clusteringInterface->Spatial_Factor<<endl;
            dropoffs = routePlannerInterface->drop_offs;
            clusteringInterface->best_Temporal_Factor = clusteringInterface->Temporal_Factor;
            clusteringInterface->best_Spatial_Factor = clusteringInterface->Spatial_Factor;
        }
        main_packages = packages;
        routePlannerInterface->drop_offs = 0;
    }

    clusteringInterface->Temporal_Factor = clusteringInterface->best_Temporal_Factor;
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
        // for(int i =0;i<cluster.size();i++){
        //     cout<<cluster[i].coordinate.latitude<<" "<<cluster[i].coordinate.longitude<<endl;
        // }
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
    cout<<"Avg ===> "<<avg/clusters.size()<<endl;
    cout<<"Max ===> "<<maximum<<endl;
    cout<<"Drop Offs => "<<routePlannerInterface->drop_offs<<endl;
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
