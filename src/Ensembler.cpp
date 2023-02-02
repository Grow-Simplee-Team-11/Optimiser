#include "../include/Ensembler.hpp"
#include <pthread.h>

namespace myNamespace{
    vector<BinPackInterface*> BinPackingInterfaces;
    vector<ClusteringInterface*> ClusteringInterfaces;
    vector<RoutePlanInterface*> RoutePlanInterfaces; 

    vector<pthread_mutex_t*> locksClustering;
    vector<pthread_mutex_t*> locksRouting;
    vector<pthread_mutex_t*> locksBinPacking;

    vector<Optimizer*> optimizersUsed;
}


Ensembler::Ensembler(vector<string>& RoutePlanningAlgorithms_, vector<string>& BinPackingAlgorithms_, vector<string>& ClusteringAlgorithms_, vector<item>& packages_, Coordinate& warehouse_, int numberRiders_, Bin& bin_){
    RoutePlanningAlgorithms = RoutePlanningAlgorithms_;
    BinPackingAlgorithms = BinPackingAlgorithms_;
    ClusteringAlgorithms = ClusteringAlgorithms_;

    if(myNamespace::RoutePlanInterfaces.empty()){
        for(auto routeAlgo :  RoutePlanningAlgorithms){
            pthread_mutex_t* lockAlgo = new pthread_mutex_t;
            pthread_mutex_init(lockAlgo, NULL);
            myNamespace::locksRouting.push_back(lockAlgo);

            cout<<routeAlgo<<" "<<(lockAlgo)<<endl;

            RoutePlanInterface* rp = NULL;
            if (routeAlgo == "TSP_OR") rp = new TSP_OR(EUCLIDEAN);
            else if (routeAlgo == "TSP_LK") rp = new TSP_LK(EUCLIDEAN);
            else if (routeAlgo == "TSP_CK") rp = new TSP(EUCLIDEAN);
            else if (routeAlgo == "TSP_OR_EDD") rp = new TSP_OR_EDD(EUCLIDEAN);
            myNamespace::RoutePlanInterfaces.push_back(rp);
        }
    }

    if(myNamespace::ClusteringInterfaces.empty()){
        for(auto clusteringAlgo :  ClusteringAlgorithms){
            pthread_mutex_t* lockAlgo = new pthread_mutex_t;
            pthread_mutex_init(lockAlgo, NULL);
            myNamespace::locksClustering.push_back(lockAlgo);
            cout<<clusteringAlgo<<" "<<(lockAlgo)<<endl;

            ClusteringInterface* cls = NULL;
            if (clusteringAlgo == "CLARKE") cls = new Clarke(EUCLIDEAN);
            else if (clusteringAlgo == "FESIF") cls = new Clarke(EUCLIDEAN);
            else if (clusteringAlgo == "SELF") cls = new SELFCLUSTERING(EUCLIDEAN);
            else if (clusteringAlgo == "HGS") cls = new HGS(EUCLIDEAN);
            myNamespace::ClusteringInterfaces.push_back(cls);
        }
    }

    if(myNamespace::BinPackingInterfaces.empty()){
        for(auto binpackingAlgo :  BinPackingAlgorithms){
            pthread_mutex_t* lockAlgo = new pthread_mutex_t;
            pthread_mutex_init(lockAlgo, NULL);
            myNamespace::locksBinPacking.push_back(lockAlgo);

            BinPackInterface* bp = NULL;
            if (binpackingAlgo == "EB_AFIT") bp = new EB_AFIT;
            myNamespace::BinPackingInterfaces.push_back(bp);
        }
    }

    packages = packages_;
    bin = bin_;
    warehouse = warehouse_;
    numberRiders = numberRiders_;
}

void Ensembler::EnsembleRun(){
    int numRoutePlanners = RoutePlanningAlgorithms.size();
    int numClusteringAlgos = ClusteringAlgorithms.size();
    int numBinpackingAlgos = BinPackingAlgorithms.size();

    vector<pthread_t> threadsRunning;

    for(int i=0;i<numRoutePlanners;i++){
        for(int j=0;j<numClusteringAlgos;j++){
            for(int k=0;k<numBinpackingAlgos;k++){
                vector<string> Combination = {RoutePlanningAlgorithms[i], ClusteringAlgorithms[j], BinPackingAlgorithms[k]};

                Ensembler* temp = new Ensembler(RoutePlanningAlgorithms, BinPackingAlgorithms, ClusteringAlgorithms, packages, warehouse, numberRiders, bin);

                temp->currentCombination = new int[3];
                temp->currentCombination[0] = i;
                temp->currentCombination[1] = j;
                temp->currentCombination[2] = k;

                pthread_t thread;
                int threadStatus = pthread_create(&thread, NULL, run, (void *)temp);
                threadsRunning.push_back(thread);
                ensemblersUsed.push_back(temp);
            }
        }
    }
    for(auto thread:threadsRunning){
        pthread_join(thread, NULL);
    }

    double minCost = 1e9;
    Optimizer* minIndex = NULL;

    for(auto tempOptimizer : (myNamespace::optimizersUsed)){
        double weight = 0.5;

        double routePlanningCost = 0.0;
        for(double cost: tempOptimizer->GetRoutingCost()){
            routePlanningCost += cost;
        }

        double packagingCost = 0.0;
        for(double cost: tempOptimizer->GetPackagingCost()){
            packagingCost += cost;
        }

        double cost = routePlanningCost + weight * packagingCost;
        if(cost < minCost){
            minCost = cost;
            minIndex = tempOptimizer;
        }
    }

    int num = minIndex->getNumClusters();

    for(int i = 0;i<num;i++){
        auto x = ((myNamespace::optimizersUsed)[0])->GetPackagingForCluster(i);
        ((myNamespace::optimizersUsed)[0])->getPackingLog(x);
    }
    vector<vector<item>> bestClusters = minIndex->GetClusters();
    int counter = 0;
    for(auto cluster : bestClusters){
        cout<<"Logging information for cluster - "<<counter<<endl;
        counter++;
        minIndex->getPackingLog(cluster);
    }

    cout<<minCost<<endl;

    for(auto ensembler: ensemblersUsed){
        Costs.push_back(ensembler->GetCosts()[0]);
    }

    return;
}

vector<pair<vector<string>, double>> Ensembler::GetCosts(){
    return Costs;
}

void Ensembler::Report(){
    for(auto x:Costs){
        string fileName = x.first[0] + x.first[1] + x.first[2];
        fileName += ".txt";

        ofstream output("./../tests/"+fileName);
        output<<x.second<<endl;
        output.close();
    }
    return;
}

void * run(void * arg){
    Ensembler* ensembler = (Ensembler*) arg;

    cout<<"Running combination - "<<ensembler->RoutePlanningAlgorithms[ensembler->currentCombination[0]]<<" "<<ensembler->ClusteringAlgorithms[ensembler->currentCombination[1]]<<" "<<ensembler->BinPackingAlgorithms[ensembler->currentCombination[2]]<<endl;;

    Optimizer* tempOptimizer = new Optimizer(myNamespace::RoutePlanInterfaces[ensembler->currentCombination[0]], myNamespace::ClusteringInterfaces[ensembler->currentCombination[1]], myNamespace::BinPackingInterfaces[ensembler->currentCombination[2]], ensembler->packages, ensembler->warehouse, ensembler->numberRiders, ensembler->bin, "", 0, 0, myNamespace::locksClustering[ensembler->currentCombination[1]], myNamespace::locksBinPacking[ensembler->currentCombination[2]], myNamespace::locksRouting[ensembler->currentCombination[0]]);

    tempOptimizer->optimize();

    (myNamespace::optimizersUsed).push_back(tempOptimizer);

    vector<string> Combination = {ensembler->RoutePlanningAlgorithms[ensembler->currentCombination[0]], ensembler->ClusteringAlgorithms[ensembler->currentCombination[1]], ensembler->BinPackingAlgorithms[ensembler->currentCombination[2]]};
    double weight = 0.5;

    double routePlanningCost = 0.0;
    for(double cost: tempOptimizer->GetRoutingCost()){
        routePlanningCost += cost;
    }

    double packagingCost = 0.0;
    for(double cost: tempOptimizer->GetPackagingCost()){
        packagingCost += cost;
    }

    (ensembler->Costs).push_back({Combination, routePlanningCost + weight * packagingCost});
}