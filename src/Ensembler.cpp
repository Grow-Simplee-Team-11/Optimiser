#include "../include/Ensembler.hpp"

Ensembler::Ensembler(vector<string>& RoutePlanningAlgorithms_, vector<string>& BinPackingAlgorithms_, vector<string>& ClusteringAlgorithms_, vector<BinPackInterface*>& BinPackingInterfaces_, vector<ClusteringInterface*>& ClusteringInterfaces_, vector<RoutePlanInterface*>& RoutePlanInterfaces_, vector<item>& packages_, Coordinate& warehouse_, int numberRiders_, Bin& bin_){
    RoutePlanningAlgorithms = RoutePlanningAlgorithms_;
    BinPackingAlgorithms = BinPackingAlgorithms_;
    ClusteringAlgorithms = ClusteringAlgorithms_;

    BinPackingInterfaces = BinPackingInterfaces_;
    ClusteringInterfaces = ClusteringInterfaces_;
    RoutePlanningInterfaces = RoutePlanInterfaces_;

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

                Ensembler* temp = new Ensembler(RoutePlanningAlgorithms, BinPackingAlgorithms, ClusteringAlgorithms, BinPackingInterfaces, ClusteringInterfaces, RoutePlanningInterfaces, packages, warehouse, numberRiders, bin);

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

        ofstream output;
        output.open("./tests/"+fileName);
        output<<x.second<<endl;
        output.close();
    }
}

void * run(void * arg){
    Ensembler* ensembler = (Ensembler*) arg;
    Optimizer* tempOptimizer = new Optimizer(ensembler->RoutePlanningInterfaces[ensembler->currentCombination[0]], ensembler->ClusteringInterfaces[ensembler->currentCombination[1]], ensembler->BinPackingInterfaces[ensembler->currentCombination[2]], ensembler->packages, ensembler->warehouse, ensembler->numberRiders, ensembler->bin, "", 0, 0);

    tempOptimizer->optimize();

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

    return;
}