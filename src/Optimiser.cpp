#include "../include/Optimiser.hpp"
#include "clustering/Clarke/clarke.hpp"
#include "routeplan/TSP_OR_EDD.hpp"
#include "binpack/EB_AFIT.hpp"

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
void multithreading(Optimizer * opt, Optimizer * temp, int thread_number){
    if(opt->clusteringInterface->clustering_method){
        vector<item> main_packages = opt->packages;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> Temporal_Factor_Gen(opt->clusteringInterface->Temporal_Factor_Lower_Limit, opt->clusteringInterface->Temporal_Factor_Upper_Limit);
        std::uniform_real_distribution<double> Spactial_Factor_Gen(opt->clusteringInterface->Spatial_Factor_Lower_Limit, opt->clusteringInterface->Spatial_Factor_Upper_Limit);
        opt->clusteringInterface->Temporal_Factor = ((1 - (1.0/(opt->dropoffs*10L + 1)))*Temporal_Factor_Gen(gen) +  ((1.0/(opt->dropoffs*10L + 1))*opt->clusteringInterface->best_Temporal_Factor));
        opt->clusteringInterface->Spatial_Factor = ((1 - (1.0/(opt->dropoffs*10L + 1)))*Spactial_Factor_Gen(gen) + ((1.0/(opt->dropoffs*10L + 1))*opt->clusteringInterface->best_Spatial_Factor));
        std::cout<<"Now starting iteration with Temporal Factor ====> "<<opt->clusteringInterface->Temporal_Factor<<endl;
        std::cout<<"Now starting iteration with Spatial Factor ====> "<<opt->clusteringInterface->Spatial_Factor<<endl;        
        assert(opt->routePlannerInterface!=NULL);
        assert(opt != NULL);
        cout<<"This is ThreadNumber " << thread_number<<endl;
        opt->routePlannerInterface->drop_offs = 0;


        opt->clusteringInterface->ComputeClusters(main_packages, opt->warehouse, opt->numberRiders, opt->bin);
        opt->clusteringInterface->CalculateCost();
        opt->clusters = opt->clusteringInterface->GetClusters();
        
        for(auto& cluster: opt->clusters){
            opt->routePlannerInterface->PlanRoute(cluster, opt->warehouse);
            opt->routePlannerInterface->CalculateCost();
        }
    }
    
}
void Optimizer::optimize(){
    cout<<"Checking Input Data ===> "<<endl;
    for(int i = 0;i<packages.size();i++){
        cout<<packages[i].size.height<<" "<<packages[i].size.length<<" "<<packages[i].size.width<<" "<<packages[i].weight<<" "<<packages[i].coordinate.latitude<<" "<<packages[i].coordinate.longitude<<" "<<packages[i].time<<endl;
    }
    cout<<"Checking Bag Info ===> "<<endl;
    cout<<bin.capacity<<" "<<bin.size.height<<" "<<bin.size.length<<" "<<bin.size.width<<endl;
    ofstream output;
    if(this->clusteringInterface->clustering_method && this->clusteringInterface->multithreading){
       
        for(int k = 0 ; k < 2; k ++){
            vector<thread> threads;
            vector<Optimizer*> optimizers;
            for(int j = 0 ; j < 5 ; j ++ ){
                RoutePlanInterface* rp = new TSP_OR_EDD(HAVERSINE);
                ClusteringInterface* cls = new Clarke(REAL);
                BinPackInterface* bp =  new EB_AFIT;

                assert(rp != NULL);
                assert(cls != NULL);
                assert(bp != NULL);

                Optimizer* tempOpt = new Optimizer(rp, cls, bp, this->packages, this->warehouse, this->numberRiders, this->bin, this->logFileName, this->verbose, this->logToFile);

                optimizers.push_back(tempOpt);
                optimizers[j]->routePlannerInterface->drop_offs = 0;
                optimizers[j]->clusteringInterface->best_Spatial_Factor = this->clusteringInterface->best_Spatial_Factor;
                optimizers[j]->clusteringInterface->best_Temporal_Factor = this->clusteringInterface->best_Temporal_Factor;
                threads.push_back(thread(multithreading,optimizers[j], this , j+1));
            }
            for(auto &th : threads){
                th.join();
            }
            for(int j = 0 ; j< optimizers.size(); j++){
                if(this->dropoffs > optimizers[j]->routePlannerInterface->drop_offs){
                    this->dropoffs = optimizers[j]->routePlannerInterface->drop_offs;
                    this->clusteringInterface->best_Spatial_Factor = optimizers[j]->clusteringInterface->best_Spatial_Factor;
                    this->clusteringInterface->best_Temporal_Factor = optimizers[j]->clusteringInterface->best_Temporal_Factor;
                }
                delete optimizers[j];
            }

        }
    output.close();
    }
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
    bool first = true;
    
    for(auto& cluster: clusters){
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