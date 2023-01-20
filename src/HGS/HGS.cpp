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
			vector<double> x_coords;
			vector<double> y_coords;
			
			Params params(cvrp.x_coords,cvrp.y_coords,cvrp.dist_mtx,cvrp.service_time,cvrp.demands,
						cvrp.vehicleCapacity,cvrp.durationLimit,nbVeh,cvrp.isDurationConstraint,verbose,ap);

			// Running HGS
			Genetic solver(params);
			solver.run();
			
			// Exporting the best solution
			if (solver.population.getBestFound() != NULL)
			{
				clusters = solver.population.getBestFound()->chromR;
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