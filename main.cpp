#include<iostream>
#include<vector>
#include "./include/clustering/fesif/fesif.hpp"
#include "./include/routeplan/TSP_OR.hpp"
#include "./include/routeplan/TSP_LK.hpp"
#include "./include/binpack/EB_AFIT.hpp"
#include "./include/Optimiser.hpp"

class DataModel{
	Coordinate warehouse;
	Bin bin;
	vector<item> packages;
};

DataModel ReadVRPs(string filename){
	DataModel dm;
	ifstream input;
	input.open(filename);
	Coordinate warehouse;
	input>>warehouse;
}

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
	
	// RoutePlanInterface* rp = new TSP_OR;
	RoutePlanInterface* rp = new TSP_LK;
	ClusteringInterface* cls = new FESIF;
	BinPackInterface* bp =  new EB_AFIT;

	bool verbose = true;
	bool logToFile = true;
	string logFileName = "FESIF_TSP_LK.txt";

	Optimizer optim(rp, cls, bp, items, warehouse, numRiders, bin, logFileName, verbose, logToFile);

	optim.optimize();
	vector<float> rcosts = optim.GetRoutingCost();
	float total_cost = 0;
	for(auto &x : rcosts)
	{
		total_cost+=x;
	}	

	if(verbose)
		std::cout<<"\nTotal Cost for routing: "<<total_cost<<" km"<<std::endl;
 	
	if(logToFile)
	{
		std::ofstream out(logFileName, std::ios_base::app);
		out<<"\nTotal Cost for routing: "<<total_cost<<" km"<<std::endl;
	}
	// f.ComputeClusters(items, warehouse, 100, bin); //wrapperLMD(items, warehouse, 100, bin);
 	// freeGlobalMemory();
 	// f.localFree();
	// f.PrintClustersToFile("clusters.txt");
 	
	// TSP_OR tsp;
	// tsp.PlanRoute(f.GetClusters()[0], f.getWarehouse());
	return 0;
 }
