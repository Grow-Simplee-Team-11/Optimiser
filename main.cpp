#include<iostream>
#include<vector>
#include "./include/clustering/fesif/fesif.hpp"
#include "./include/routeplan/TSP_OR.hpp"
#include "./include/binpack/EB_AFIT.hpp"
#include "./include/Optimiser.hpp"

int main(int argc, char** argv) {
    
 	ifstream input;
 	input.open("input.txt");
 	Coordinate warehouse;
 	input>>warehouse.longitude>>warehouse.latitude;
 	Bin bin;
 	input>>bin.size.length>>bin.size.width>>bin.size.height;
 	bin.capacity = 25;
 	int n;
 	input>>n;
 	vector<item> items(n);
 	for(int i=0;i<n;i++) {
 		input>>items[i].coordinate.longitude>>items[i].coordinate.latitude;
 		input>>items[i].size.length>>items[i].size.width>>items[i].size.height;
 		items[i].weight = 1;
 	}
	int numRiders = 5;
	
	RoutePlanInterface* rp = new TSP_OR;
	ClusteringInterface* cls = new FESIF;
	BinPackInterface* bp =  new EB_AFIT;
	Optimizer optim(rp, cls, bp, items, warehouse, numRiders, bin, "FESIF_TSP.txt", false, true);

	optim.optimize();
 	// f.ComputeClusters(items, warehouse, 100, bin); //wrapperLMD(items, warehouse, 100, bin);
 	// freeGlobalMemory();
 	// f.localFree();
	// f.PrintClustersToFile("clusters.txt");
 	
	// TSP_OR tsp;
	// tsp.PlanRoute(f.GetClusters()[0], f.getWarehouse());
	return 0;
 }
