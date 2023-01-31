// Hybrid Genetic Search-CVRP (HGS) Algorithm computes optimal clusters and routes simultaneously
#include "../../../include/clustering/HGS/HGS.hpp"

/**
 * @brief Construct a new HGS::HGS object 
 * 
 * @param method 
 */
HGS::HGS(DistanceType method,bool EDD): ClusteringInterface(method){
			// pathInstance = instance_path_name;
			// pathSolution = solution_path_name;
			this->EDD = EDD;
			isRoundingInteger = false;
			verbose = true;
			pathSolution = "../tests/HGS_sol.txt";
			ap = default_algorithm_parameters();
			// ap.timeLimit = 30.0;
			ap.nbIter = 25000;
}

/**
 * @brief evaluate and output details of location cluster that each rider is assigned to
 * 
 * @param packages details of packages to be delivered
 * @param warehouse depot/ warehouse information
 * @param numRiders number of riders
 * @param b details of bin/ rider bag
 */
void HGS::ComputeClusters(vector<item> &packages, Coordinate warehouse, int numRiders, Bin b) {
							
	int nbVeh = numRiders;
	// InstanceCVRPLIB cvrp(pathInstance, isRoundingInteger);
	int n = packages.size();
	// double time_to_deliver_in_km = 1.5; 
	double time_to_deliver_in_km = 0; 
	vector<double> x_coords(n+1);
	vector<double> y_coords(n+1);
	vector<double> demands(n+1,0.);
	vector<double> service_time(n+1,time_to_deliver_in_km);
	x_coords[0] = warehouse.latitude;
	y_coords[0] = warehouse.longitude;
	std::vector < std::vector< double > > dist_mtx = std::vector < std::vector< double > >(n + 1, std::vector <double>(n + 1));
	for(int i = 1;i <= n;i++){
		x_coords[i] = packages[i-1].coordinate.latitude;
		y_coords[i] = packages[i-1].coordinate.longitude;
		demands[i] = packages[i-1].getVolume();
	}
	for (int i = 0; i <= n; i++){
		for (int j = 0; j <= n; j++){
			if(i != 0 && j != 0)
				dist_mtx[i][j] = Dist(packages[i-1].coordinate,packages[j-1].coordinate);
			else if(i == 0 && j != 0)
				dist_mtx[i][j] = Dist(warehouse,packages[j-1].coordinate);
			else if(j == 0 && i != 0)
				dist_mtx[i][j] = Dist(packages[i-1].coordinate,warehouse);
			else dist_mtx[i][j] = 0;
			// dist_mtx[i][j] = Dist(packages[i].coordinate,packages[j].coordinate);
			if (isRoundingInteger) dist_mtx[i][j] = round(dist_mtx[i][j]);
		}
	}
	cout << "X : ";
	for(int i = 0;i < x_coords.size();i++){
		cout << x_coords[i]<< ' ';
	}
	cout << "\nY : ";
	for(int i = 0;i < y_coords.size();i++){
		cout << y_coords[i]<< ' ';
	}
	cout<<"\nD : ";
	for(int i = 0;i < demands.size();i++){
		cout << demands[i]<< ' ';
	}
	cout << endl;
	std::vector<double> expectation(n); 
	// TODO: add expectation in HGS
	for(int i=0;i<n;i++) expectation[i] = packages[i].time;
	double avgspeed = 100.0;
	double DurationLimit = avgspeed*5;//Duration Limit is given in kms
	
	if(this->EDD){
		Params params = Params(x_coords,y_coords,dist_mtx,service_time,demands,
				b.capacity,DurationLimit,numRiders,true,verbose, expectation,ap);
		params.averageSpeed = avgspeed;
		print_algorithm_parameters(ap);
		Genetic solver(params);
		solver.run();
		cout << "Solver Completed"<<endl;
		// Exporting the best solution
		const Individual indiv = *solver.population.getBestFound();
		if (solver.population.getBestFound() != NULL)
		{
			for (int k = 0; k < (int)indiv.chromR.size(); k++)
			{
				if (!indiv.chromR[k].empty())
				{
					vector<item> cluster;
					for (int i : indiv.chromR[k]){
						if(i != 0)
							cluster.push_back(packages[i-1]);
					} 
					clusters.push_back(cluster);
				}
			}
			if (params.verbose) std::cout << "----- WRITING BEST SOLUTION IN : " << pathSolution << std::endl;
			solver.population.exportCVRPLibFormat(*solver.population.getBestFound(), pathSolution);
			solver.population.exportSearchProgress(pathSolution + ".PG.csv", pathInstance);
		}
	}
	else{
		Params params(x_coords,y_coords,dist_mtx,service_time,demands,
				b.capacity,1.e30,numRiders,false,verbose, expectation,ap);
		print_algorithm_parameters(ap);
		Genetic solver(params);
		solver.run();
		cout << "Solver Completed"<<endl;
		// Exporting the best solution
		const Individual indiv = *solver.population.getBestFound();
		if (solver.population.getBestFound() != NULL)
		{
			for (int k = 0; k < (int)indiv.chromR.size(); k++)
			{
				if (!indiv.chromR[k].empty())
				{
					vector<item> cluster;
					for (int i : indiv.chromR[k]){
						if(i != 0)
							cluster.push_back(packages[i-1]);
					} 
					clusters.push_back(cluster);
				}
			}
			if (params.verbose) std::cout << "----- WRITING BEST SOLUTION IN : " << pathSolution << std::endl;
			solver.population.exportCVRPLibFormat(*solver.population.getBestFound(), pathSolution);
			solver.population.exportSearchProgress(pathSolution + ".PG.csv", pathInstance);
		}
	}			
}

void HGS::CalculateCost() {}