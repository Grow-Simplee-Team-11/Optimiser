#include <iostream>
#include <vector>
#include "src/clustering/Clarke/clarke.hpp"
#include "src/routeplan/tsp.h"
#include <climits>


using namespace std;

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
	
	for(int i =0;i< sol.clusters.size();i++){
		if(sol.clusters[i].size()>1){
			TSP tsp;
			// cout<<"Now Planning Route For =====>\n";
			// for(int j =0;j < sol.clusters[i].size();j++)
			// 	cout<<sol.clusters[i][j].coordinate.latitude<<" "<<sol.clusters[i][j].coordinate.longitude<<endl;
			// cout<<"With WareHouse at ======>\n";
			// cout<<sol.warehouse.latitude<<" "<<sol.warehouse.longitude<<endl;
			tsp.PlanRoute(sol.clusters[i], sol.warehouse, sol.b);
		}
	}
    return 0;
}