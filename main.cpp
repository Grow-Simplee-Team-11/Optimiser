#include<iostream>
#include<vector>
#include "./include/clustering/fesif/fesif.hpp"
#include "./include/clustering/HGS/HGS.hpp"
#include "./src/clustering/Clarke/clarke.hpp"


#include "./include/routeplan/TSP_OR.hpp"
#include "./include/routeplan/TSP_OR_EDD.hpp"
#include "./include/routeplan/TSP_LK.hpp"
#include "./include/clustering/HGS/HGS.hpp"
#include "./src/routeplan/tsp.h"

#include "./include/binpack/EB_AFIT.hpp"
#include "./include/Optimiser.hpp"
#include <ctime>
class DataModel{
	public:
	Coordinate warehouse;
	Bin bin;
	vector<item> packages;
	int numRiders = 5;
};

DataModel ReadVRPs(string filename){
	DataModel dm;
	ifstream inputFile;
	inputFile.open(filename);
	int index = 0;
	cout << 'a';
	while(filename[index] != 'k')
		index++;
	index++;
	string nums;
	cout << 'b';
	while(filename[index] != '.'){
		nums.push_back(filename[index]);
		index++;
	}
	cout << 'c';
	dm.numRiders = stoi(nums);
	cout << nums << dm.numRiders << endl;
	if(!inputFile.is_open()){
		std::cout<<"Cannot Open File\n";
	}
	Coordinate warehouse;

	std::string content, content2, content3;
	double serviceTimeData = 0.;

	getline(inputFile, content);
	getline(inputFile, content);
	getline(inputFile, content);
	int nbClients;int durationLimit;
	double vehicleCapacity = 1.e30;
	bool isDurationConstraint;
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
	
	std::vector<double> x_coords = std::vector<double>(nbClients);
	std::vector<double> y_coords = std::vector<double>(nbClients);
	std::vector<double> demands = std::vector<double>(nbClients);
	std::vector<double> service_time = std::vector<double>(nbClients);

	// Reading node coordinates
	// depot must be the first element
	// 		- i = 0 in the for-loop below, or
	// 		- node_number = 1 in the .vrp file
	// customers are
	// 		- i = 1, 2, ..., nbClients in the for-loop below, or
	// 		- node_number = 2, 3, ..., nb_Clients in the .vrp file
	int node_number;
	int depot_lat,depot_lon;
	inputFile >> node_number >> depot_lat>> depot_lon ;
	for (int i = 0; i < nbClients; i++)
	{
		inputFile >> node_number >> x_coords[i] >> y_coords[i];
		if (node_number != i + 2) throw std::string("The node numbering is not in order.");
	}
	cout << "coordinates taken" << endl;
	// Reading demand information
	inputFile >> content;
	if (content != "DEMAND_SECTION") throw std::string("Unexpected data in input file: " + content);

	inputFile >> content >> content;
	for (int i = 0; i < nbClients; i++)
	{
		inputFile >> node_number >> demands[i];
		if (node_number != i + 2) throw std::string("The node numbering is not in order.");
		// service_time[i] = (i == 0) ? 0. : serviceTimeData ;
	}

	
	cout << "demands taken" << endl;
	// Reading depot information (in all current instances the depot is represented as node 1, the program will return an error otherwise)
	inputFile >> content >> content2 >> content3 >> content3;
	if (content != "DEPOT_SECTION") throw std::string("Unexpected data in input file: " + content);
	if (content2 != "1") throw std::string("Expected depot index 1 instead of " + content2);
	if (content3 != "EOF") throw std::string("Unexpected data in input file: " + content3);

	
	// add items to datamodel
	dm.packages.resize(nbClients);
	dm.bin.capacity = vehicleCapacity;
	dm.bin.size.height = 1e5;
	dm.bin.size.length = 1e5;
	dm.bin.size.width = 1e5;
	dm.warehouse.latitude  = depot_lat;
	dm.warehouse.longitude = depot_lon;
	srand(time(0));
	int lb = 9, ub = 13;
	for(int i=0; i<nbClients; i++)
	{
		dm.packages[i].coordinate.latitude = x_coords[i];
		dm.packages[i].coordinate.longitude = y_coords[i];
		dm.packages[i].volume = 1;
		dm.packages[i].weight = demands[i];
		dm.packages[i].time = ((rand() % (ub - lb + 1)) + lb)*60 ;
	}
	
	return dm;
	// input>>warehouse;
}

// int main(int argc, char** argv) {
    
//  	// ifstream input;
//  	// input.open("./tests/output.txt");
// 	fstream input("./tests/output.txt");

// 	cout<<"Opened File"<<endl;
//  	Coordinate warehouse;
//  	input>>warehouse.longitude>>warehouse.latitude;
//  	Bin bin;
//  	input>>bin.size.length>>bin.size.width>>bin.size.height;
//  	int n;
//  	input>>n;
//  	vector<item> items(n);
// 	cout<<"Started Taking Input"<<endl;
//  	for(int i=0;i<n;i++) {
//  		input>>items[i].coordinate.longitude>>items[i].coordinate.latitude;
//  		input>>items[i].size.length>>items[i].size.width>>items[i].size.height;
// 		double t;
// 		input>>t;
// 		items[i].time = (t-1665532800)/60; 
// 		cout<<items[i].time<<endl;
// 		items[i].volume = items[i].size.length * items[i].size.width * items[i].size.height;
//  		items[i].weight = 1;

// 		// cout << items[i].coordinate.latitude << " " << items[i].coordinate.longitude << " " <<items[i].size.length<< " " << items[i].size.widthitems[i].size.height << " " << items[i].time << endl;
//  	}
// 	cout<<"Finished Taking Input"<<endl;
// 	int numRiders = 5;
// 	RoutePlanInterface* rp = new TSP_OR_EDD(HAVERSINE);
// 	ClusteringInterface* cls = new Clarke(HAVERSINE);
// 	BinPackInterface* bp =  new EB_AFIT;
// 	cout<<"Called Optimised"<<endl;
// 	Optimizer optim(rp, cls, bp, items, warehouse, numRiders, bin,"FESIF_TSP.txt", true, true);

// 	optim.optimize();
// 	vector<float> costs = optim.GetRoutingCost();
// 	float total_cost = 0;
// 	for(int i = 0 ; i< costs.size();i++)
// 		total_cost+=costs[i];
// 	cout<<"Total Cost of All Routes is ===> "<< total_cost<<endl;
// 	return 0;
//  }

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
		// dm = ReadVRPs(s);
	}
	// string s;
	// cin >> s;
	// cout << s << endl;
	// try{
	// 	dm = ReadVRPs(s);
	// }
	// catch(string err){
	// 	cout << err << endl;
	// }
	cout << "Capacity : " << dm.bin.capacity << " depoX :" << dm.warehouse.latitude <<" depoY : " << dm.warehouse.longitude << endl;
	for(int i = 0;i < dm.packages.size();i++){
		cout << i << " : x : " << dm.packages[i].coordinate.latitude << " y : " << dm.packages[i].coordinate.longitude << " weight : "<< dm.packages[i].weight << endl;
	}
	RoutePlanInterface* rp = new TSP_OR_EDD(EUCLIDEAN);
	// RoutePlanInterface* rp = new TSP_LK;
	// ClusteringInterface* cls = new FESIF;
	// ClusteringInterface* cls = new Clarke(EUCLIDEAN);
	ClusteringInterface* cls = new Clarke(EUCLIDEAN);
	BinPackInterface* bp =  new EB_AFIT;

	bool verbose = true;
	bool logToFile = true;
	string logFileName = "FESIF_TSP_LK.txt";

	// int numRiders = 5;
	Optimizer optim(rp, cls, bp, dm.packages, dm.warehouse, dm.numRiders, dm.bin, logFileName, verbose, logToFile);

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

	return 0;
}
