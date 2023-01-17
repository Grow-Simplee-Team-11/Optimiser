#include<iostream>
#include<vector>
#include "./include/clustering/fesif/fesif.hpp"
#include "./src/clustering/Clarke/clarke.hpp"

#include "./include/routeplan/TSP_OR.hpp"
#include "./include/routeplan/TSP_LK.hpp"
#include "./src/routeplan/tsp.h"

#include "./include/binpack/EB_AFIT.hpp"
#include "./include/Optimiser.hpp"

class DataModel{
	public:
	Coordinate warehouse;
	Bin bin;
	vector<item> packages;
};

DataModel ReadVRPs(string filename){
	DataModel dm;
	ifstream inputFile;
	inputFile.open(filename);
	if(!inputFile.is_open()){
		std::cout<<"Cannot Open File\n";
	}


	inputFile >> dm.warehouse.latitude >> dm.warehouse.longitude ;

	inputFile >> dm.bin.size.length >> dm.bin.size.width >> dm.bin.size.height;
	int num_points;
	inputFile >> num_points;
	dm.packages.resize(num_points);
	for(int i = 0;i < num_points;i++){
		double a,b,c;
		inputFile >> dm.packages[i].coordinate.latitude >> dm.packages[i].coordinate.longitude >> a >> b >> c;
		dm.packages[i].volume = a*b*c;
	}
	return dm;
	// input>>warehouse;
}


int main(int argc, char** argv){
	DataModel dm;
	
	if(argv[1] != NULL){
		try{
			dm = ReadVRPs(argv[1]);
		}
		catch(string err){
			cout << err;
		}
		
	}
	else{
		string s;
		cin >> s;
		try{
			dm = ReadVRPs(s);
		}
		catch(string err){
			cout << err;
		}
	}

	cout << "Capacity : " << dm.bin.capacity << " depoX :" << dm.warehouse.latitude <<" depoY : " << dm.warehouse.longitude << endl;
	for(int i = 0;i < dm.packages.size();i++){
		cout << i << " : x : " << dm.packages[i].coordinate.latitude << " y : " << dm.packages[i].coordinate.longitude << " weight : "<< dm.packages[i].weight << endl;
	}
	RoutePlanInterface* rp = new TSP_OR(HAVERSINE);
	// RoutePlanInterface* rp = new TSP(HAVERSINE);
	// RoutePlanInterface* rp = new TSP_LK;
	// ClusteringInterface* cls = new FESIF;
	ClusteringInterface* cls = new Clarke(HAVERSINE);
	BinPackInterface* bp =  new EB_AFIT;

	bool verbose = true;
	bool logToFile = true;
	string logFileName = "FESIF_TSP_LK.txt";

	int numRiders = 5;
	Optimizer optim(rp, cls, bp, dm.packages, dm.warehouse, numRiders, dm.bin, logFileName, verbose, logToFile);

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