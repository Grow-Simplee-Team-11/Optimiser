/*************************************************************************
Title: TSP.hpp
Description: TSP class specification file for our Christofides implementation
Authors: Sean Hinds, Ryan Hong, Jeff Herlitz
Date: 08/16/17
*************************************************************************/

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <stdio.h>
#include <vector>
#include <limits>
#include "../../include/interface/RoutePlanInterface.hpp"
using namespace std;

#ifndef TSP_H
#define TSP_H

class TSP :public RoutePlanInterface{
private:
	// List of odd nodes
	vector<int>odds;

	//Smaller cost matrix to find minimum matching on odd nodes
	int **cost;

	//Adjacency list
	vector<int> *adjList;

	void findOdds();



protected:


public:
	// Number of cities
	int n;

	//path
	int **path_vals;

	//Shortest path length
	int pathLength;

	//euler circuit
	vector<int> circuit;

	vector<item> packages;

	// n x n, pairwise distances between cities
	int **graph;

  vector<int>* adjlist;

	// Constructor
	TSP();

	// Destructor
	~TSP();

	double get_distance(item p1, item p2);

	//Find perfect matching
	void perfectMatching();

	//Find Euler tour
	void euler_tour(int start, vector<int> &path);

	//Find Hamiltonian path
	void make_hamiltonian(vector<int> &path, int &pathCost);

	// Prim's algorithm
	void findMST();

	int getMinIndex(int key[], bool mst[]);

	void printResult();
	void printPath();
	void printEuler();
	void printAdjList();

	int get_size(){return n;};

	void fillMatrix();

	int findBestPath(int start);
    void PlanRoute(vector<item> cluster, Coordinate warehouse, Bin b);
    float CalculateCost();
};

#endif