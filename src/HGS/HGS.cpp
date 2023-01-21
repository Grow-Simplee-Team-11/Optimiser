// Hybrid Genetic Search-CVRP Algorithm computes optimal clusters and routes simultaneously
#include "../../include/interface/ClusteringInterface.hpp"
// #include "commandline.h"
#include "../../include/clustering/HGS/AlgorithmParameters.h"
#include "../../include/clustering/HGS/Genetic.h"
#include "../../include/clustering/HGS/LocalSearch.h"
#include "../../include/clustering/HGS/Split.h"
#include "../../include/clustering/HGS/InstanceCVRPLIB.h"

#include <iostream>
#include <string>
#include <climits>


using namespace std;

class HGS : public ClusteringInterface
{
	public:

		const int SCALING_FACTOR=100; 
		
		HGS(bool method): ClusteringInterface(method){
			// pathInstance = instance_path_name;
			// pathSolution = solution_path_name;
			isRoundingInteger = true;
			verbose = true;
			pathSolution = "./tests/HGS_sol.txt";
			ap = default_algorithm_parameters();
		}
		double Dist(struct Coordinate c1,struct Coordinate c2){
            if(method == EUCLIDEAN)
                return euclidean(c1,c2);
            else if(method == HAVERSINE) 
                return haversine(c1,c2)*SCALING_FACTOR;
        }
		
		void ComputeClusters(vector<item> packages, Coordinate warehouse, int numRiders, Bin b) {
							
			int nbVeh = numRiders;
			InstanceCVRPLIB cvrp(pathInstance, isRoundingInteger);
			int n = packages.size();
			vector<double> x_coords(n+1);
			vector<double> y_coords(n+1);
			vector<double> demands(n+1,0.);
			vector<double> service_time(n+1,0.);
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
					dist_mtx[i][j] = std::sqrt(
						(x_coords[i] - x_coords[j]) * (x_coords[i] - x_coords[j])
						+ (y_coords[i] - y_coords[j]) * (y_coords[i] - y_coords[j])
					);

					if (isRoundingInteger) dist_mtx[i][j] = round(dist_mtx[i][j]);
				}
			}
			

			// Params params(x_coords,y_coords,)
			// Params params(cvrp.x_coords,cvrp.y_coords,cvrp.dist_mtx,cvrp.service_time,cvrp.demands,
						// cvrp.vehicleCapacity,cvrp.durationLimit,nbVeh,cvrp.isDurationConstraint,verbose,ap);
			Params params(x_coords,y_coords,dist_mtx,service_time,demands,
						b.capacity,1.e30,numRiders,false,verbose,ap);
			// Running HGS
			Genetic solver(params);
			solver.run();
			
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

		void CalculateCost() {}


	private:
		string pathInstance;		// Instance path
		string pathSolution;		// Solution path
		bool isRoundingInteger;
		bool verbose;
		AlgorithmParameters ap;
		
};