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
void Optimizer::check_data(){
    if(warehouse.latitude <= 0 || warehouse.longitude <= 0){
        throw "Warehouse coordinates are not valid";
    }
    if(numberRiders <= 0){
        throw "Number of riders are not valid";
    }
    if(packages.size() == 0){
        throw "No packages to optimize";
    }
    if(bin.size.length < 0 || bin.size.width < 0 || bin.size.height < 0 || bin.capacity < 0 || bin.getVolume() < 0){
        throw "Bin dimensions are not valid";
    }
    for(auto& package: packages){
        if(package.coordinate.latitude <= 0 || package.coordinate.longitude <= 0){
            throw "Package coordinates are not valid";
        }
        if(package.size.length <= 0 || package.size.width <= 0 || package.size.height <= 0 || package.getVolume() <= 0){
            throw "Package dimensions are not valid";
        }
        if(package.weight <= 0){
            throw "Package weight is not valid";
        }
    }

}
void Optimizer::optimize(){
    check_data();
    clusteringInterface->ComputeClusters(packages, warehouse, numberRiders, bin);
    if(clusteringInterface->GetClusters().size() == 0){
        throw "Clustering Algorithm Could Not found a solution";
    }
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
    bool first = true;
    ofstream output;
    clusterPaths.clear();
    for(int i=0; i< clusters.size();i++){
        vector<item>& cluster = clusters[i];

        if(verbose){
            cout<<"Printing information for cluster - "<<i<<endl;
        }
        if(logToFile){
            std::ofstream out(logFileName, std::ios_base::app);
            out<<"Printing information for cluster - "<<i<<endl;
        }
        i++;

        // Planning routes
        // for(int i =0;i<cluster.size();i++){
        //     cout<<cluster[i].coordinate.latitude<<" "<<cluster[i].coordinate.longitude<<endl;
        // }
        routePlannerInterface->PlanRoute(cluster, warehouse);
        vector<item> rps = routePlannerInterface->GetPaths();
        clusterPaths.push_back(rps);
        routePlannerInterface->CalculateCost();
        routePlanningCost.push_back(routePlannerInterface->GetPathPlanningCost());

            if(verbose){
                routePlannerInterface->PrintRoutes();
            }
            if(logToFile){
                routePlannerInterface->PrintRoutesToFile(logFileName);
            }
            // Computing bin packaging
            binPackInterface->BinPack(rps, bin);
            
            clusterPackagings.push_back(binPackInterface->GetPackaging());
            binPackInterface->CalculateCost();
            packagingCost.push_back(binPackInterface->CalculateCost());
            first = true;

            output.open("./cuboids_to_render.xml");
            output<<"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>"<<endl;
            output<<"<!DOCTYPE boost_serialization>"<<endl;
            output<<"<boost_serialization signature=\"serialization::archive\" version=\"10\">"<<endl;
            for(int j = 0 ; j < cluster.size(); j++){
                first == true ? output<<"<cuboid class_id=\"0\" tracking_level=\"0\" version=\"0\">"<<endl : output<<"<cuboid>"<<endl;
                first = false;
                output<<"<width>"<<cluster[j].size.width<<"</width>"<<endl;
                output<<"<height>"<<cluster[j].size.height<<"</height>"<<endl;
                output<<"<depth>"<<cluster[j].size.length<<"</depth>"<<endl;
                output<<"<x>"<<cluster[j].position.x<<"</x>"<<endl;
                output<<"<y>"<<cluster[j].position.y<<"</y>"<<endl;
                output<<"<z>"<<cluster[j].position.z<<"</z>"<<endl;
                output<<"</cuboid>"<<endl;
            }
            output<<"<base class_id=\"1\" tracking_level=\"0\" version=\"0\">"<<endl;
            output<<"<width>"<<bin.size.width<<"</width>"<<endl;
            output<<"<height>"<<bin.size.length<<"</height>"<<endl;
            output<<"<x>0</x>"<<endl;
            output<<"<y>0</y>"<<endl;
            output<<"</base>"<<endl;
            output<<"</boost_serialization>"<<endl;
            output.close();
            if(verbose){
                binPackInterface->PrintPackedData();
            }
            if(logToFile){
                binPackInterface->PrintPackedDataToFile(logFileName);
            }
            avg+= cluster.size();
            maximum = max(maximum, (int)cluster.size());

    }
    output.open("./tests/cluster.txt");
    output<<clusterPaths.size()<<"\n";
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
    output << warehouse.latitude <<" "<< warehouse.longitude<<endl;
    
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
int Optimizer::getNumClusters(){
    return clusterPackagings.size();
}