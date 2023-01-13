#include<iostream>
#include<vector>
#include "./include/clustering/fesif/fesif.hpp"

 int main(int argc, char** argv) {
    FESIF f;
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

 	f.ComputeClusters(items, warehouse, 100, bin); //wrapperLMD(items, warehouse, 100, bin);
 	// freeGlobalMemory();
 	// f.localFree();
	f.PrintClustersToFile("output.txt");
 	return 0;
 }
