#include<iostream>
#include<vector>
#include "src/clustering/Clarke/clarke.hpp"

int main(){
    Clarke sol;
    ifstream input;
 	input.open("input.txt");
 	Coordinate warehouse;
 	input>>warehouse.longitude>>warehouse.latitude;
 	Bin bin;
 	input>>bin.size.length>>bin.size.width>>bin.size.height;
 	bin.capacity = 250;
 	int n;
 	input>>n;
 	vector<item> items(n);
 	for(int i=0;i<n;i++) {
 		input>>items[i].coordinate.longitude>>items[i].coordinate.latitude;
 		input>>items[i].size.length>>items[i].size.width>>items[i].size.height;
 		items[i].weight = 1;
 	}
    // sol.test();
 	sol.ComputeClusters(items, warehouse, 100, bin, 22); //wrapperLMD(items, warehouse, 100, bin);
    return 0;
}