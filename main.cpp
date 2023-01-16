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

	Coordinate warehouse;

	std::string content, content2, content3;
	double serviceTimeData = 0.;

	getline(inputFile, content);
	getline(inputFile, content);
	getline(inputFile, content);
	for (inputFile >> content ; content != "NODE_COORD_SECTION" ; inputFile >> content)
	{
		if (content == "DIMENSION") { inputFile >> content2 >> nbClients; nbClients--; } // Need to substract the depot from the number of nodes
		else if (content == "EDGE_WEIGHT_TYPE")	inputFile >> content2 >> content3;
		else if (content == "CAPACITY")	inputFile >> content2 >> vehicleCapacity;
		else if (content == "DISTANCE") { inputFile >> content2 >> durationLimit; isDurationConstraint = true; }
		else if (content == "SERVICE_TIME")	inputFile >> content2 >> serviceTimeData;
		else throw std::string("Unexpected data in input file: " + content);
	}
	if (nbClients <= 0) throw std::string("Number of nodes is undefined");
	if (vehicleCapacity == 1.e30) throw std::string("Vehicle capacity is undefined");

	std::vector<double> x_coords = std::vector<double>(nbClients + 1);
	std::vector<double> y_coords = std::vector<double>(nbClients + 1);
	std::vector<double> demands = std::vector<double>(nbClients + 1);
	std::vector<double> service_time = std::vector<double>(nbClients + 1);

	// Reading node coordinates
	// depot must be the first element
	// 		- i = 0 in the for-loop below, or
	// 		- node_number = 1 in the .vrp file
	// customers are
	// 		- i = 1, 2, ..., nbClients in the for-loop below, or
	// 		- node_number = 2, 3, ..., nb_Clients in the .vrp file
	int node_number;
	for (int i = 0; i <= nbClients; i++)
	{
		inputFile >> node_number >> x_coords[i] >> y_coords[i];
		if (node_number != i + 1) throw std::string("The node numbering is not in order.");
	}

	// Reading demand information
	inputFile >> content;
	if (content != "DEMAND_SECTION") throw std::string("Unexpected data in input file: " + content);
	for (int i = 0; i <= nbClients; i++)
	{
		inputFile >> content >> demands[i];
		service_time[i] = (i == 0) ? 0. : serviceTimeData ;
	}

	// Calculating 2D Euclidean Distance
	// dist_mtx = std::vector < std::vector< double > >(nbClients + 1, std::vector <double>(nbClients + 1));
	// for (int i = 0; i <= nbClients; i++)
	// {
	// 	for (int j = 0; j <= nbClients; j++)
	// 	{
	// 		dist_mtx[i][j] = std::sqrt(
	// 			(x_coords[i] - x_coords[j]) * (x_coords[i] - x_coords[j])
	// 			+ (y_coords[i] - y_coords[j]) * (y_coords[i] - y_coords[j])
	// 		);

	// 		if (isRoundingInteger) dist_mtx[i][j] = round(dist_mtx[i][j]);
	// 	}
	// }

	// Reading depot information (in all current instances the depot is represented as node 1, the program will return an error otherwise)
	inputFile >> content >> content2 >> content3 >> content3;
	if (content != "DEPOT_SECTION") throw std::string("Unexpected data in input file: " + content);
	if (content2 != "1") throw std::string("Expected depot index 1 instead of " + content2);
	if (content3 != "EOF") throw std::string("Unexpected data in input file: " + content3);

	dm.warehouse.
	// add items to datamodel
	for(int i=0; i<=nbClients; i++)
	{
		
	}

	// input>>warehouse;
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
		items[i].volume = items[i].size.length * items[i].size.width * items[i].size.height;
 		items[i].weight = 1;
 	}
	int numRiders = 5;
	
	// RoutePlanInterface* rp = new TSP_OR;
	// RoutePlanInterface* rp = new TSP_LK;
	// ClusteringInterface* cls = new FESIF;
	// BinPackInterface* bp =  new EB_AFIT;

	// bool verbose = true;
	// bool logToFile = true;
	// string logFileName = "FESIF_TSP_LK.txt";

	// Optimizer optim(rp, cls, bp, items, warehouse, numRiders, bin, logFileName, verbose, logToFile);

	// optim.optimize();
	// vector<float> rcosts = optim.GetRoutingCost();
	// float total_cost = 0;
	// for(auto &x : rcosts)
	// {
	// 	total_cost+=x;
	// }	

	// if(verbose)
	// 	std::cout<<"\nTotal Cost for routing: "<<total_cost<<" km"<<std::endl;
 	
	// if(logToFile)
	// {
	// 	std::ofstream out(logFileName, std::ios_base::app);
	// 	out<<"\nTotal Cost for routing: "<<total_cost<<" km"<<std::endl;
	// }
	// f.ComputeClusters(items, warehouse, 100, bin); //wrapperLMD(items, warehouse, 100, bin);
	RoutePlanInterface* rp = new TSP;
	ClusteringInterface* cls = new Clarke;
	BinPackInterface* bp =  new EB_AFIT;
	Optimizer optim(rp, cls, bp, items, warehouse, numRiders, bin,"FESIF_TSP.txt", false, true);

	optim.optimize();
	vector<float> costs = optim.GetRoutingCost();
	float total_cost = 0;
	for(int i = 0 ; i< costs.size();i++)
		total_cost+=costs[i];
	cout<<"Total Cost of All Routes is ===> "<< total_cost<<endl;
 	// f.ComputeClusters(items, warehouse, 100, bin); //wrapperLMD(items, warehouse, 100, bin);
 	// freeGlobalMemory();
 	// f.localFree();
	// f.PrintClustersToFile("clusters.txt");
 	
	// TSP_OR tsp;
	// tsp.PlanRoute(f.GetClusters()[0], f.getWarehouse());
	return 0;
 }
