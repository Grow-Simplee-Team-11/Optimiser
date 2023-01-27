// Hybrid Genetic Search-CVRP Algorithm computes optimal clusters and routes simultaneously
#include "../../../include/clustering/HGS/HGS.hpp"

HGS::HGS(bool method): ClusteringInterface(method){
			// pathInstance = instance_path_name;
			// pathSolution = solution_path_name;
			isRoundingInteger = false;
			verbose = true;
			pathSolution = "./tests/HGS_sol.txt";
			ap = default_algorithm_parameters();
			// ap.timeLimit = 30.0;
			ap.nbIter = 10000;
}
void HGS::ComputeClusters(vector<item> &packages, Coordinate warehouse, int numRiders, Bin b) {
							
			int nbVeh = numRiders;
			// InstanceCVRPLIB cvrp(pathInstance, isRoundingInteger);
			int n = packages.size();
			double time_to_deliver_in_km = 1.5; 
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
				demands[i] = packages[i-1].weight;
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

			double DurationLimit = 30.0;
			////////////Uncomment for duration limit//////////////////
			// Params params(x_coords,y_coords,dist_mtx,service_time,demands,
			// 			b.capacity,DurationLimit,numRiders,true,verbose, expectation,ap);
			///////////////////////////////////////////////////////
			Params params(x_coords,y_coords,dist_mtx,service_time,demands,
						b.capacity,1.e30,numRiders,false,verbose, expectation,ap);
			//////////////////////////////////////
			print_algorithm_parameters(ap);
			// Running HGS
			Genetic solver(params);
			solver.run();
			cout << "Solver Completed"<<endl;
			// Exporting the best solution
			const Individual indiv = *solver.population.getBestFound();
			if (solver.population.getBestFound() != NULL)
			{
				// clusters = solver.population.getBestFound()->chromR;

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

void HGS::CalculateCost() {}