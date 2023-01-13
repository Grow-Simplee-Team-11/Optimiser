// Hybrid Genetic Search-CVRP Algorithm computes optimal clusters and routes simultaneously
#include "../../../include/interface/ClusteringInterface.hpp"
// #include "commandline.h"
#include "AlgorithmParameters.h"
#include "Genetic.h"
#include "LocalSearch.h"
#include "Split.h"
#include "InstanceCVRPLIB.h"

#include <iostream>
#include <string>
#include <climits>


using namespace std;

class HGS : public ClusteringInterface
{
	public:

		
		HGS(string instance_path_name, string solution_path_name, ){
			pathInstance = instance_path_name;
			pathSolution = solution_path_name;
			isRoundingInteger = true;
			verbose = true;

			ap = default_algorithm_parameters();


			// std::cout << "-------------------------------------------------- HGS-CVRP algorithm (2020) ---------------------------------------------------" << std::endl;
			// std::cout << "Call with: ./hgs instancePath solPath [-it nbIter] [-t myCPUtime] [-seed mySeed] [-veh nbVehicles] [-log verbose]               " << std::endl;
			// std::cout << "[-it <int>] sets a maximum number of iterations without improvement. Defaults to 20,000                                         " << std::endl;
			// std::cout << "[-t <double>] sets a time limit in seconds. If this parameter is set the code will be run iteratively until the time limit      " << std::endl;
			// std::cout << "[-seed <int>] sets a fixed seed. Defaults to 0                                                                                  " << std::endl;
			// std::cout << "[-veh <int>] sets a prescribed fleet size. Otherwise a reasonable UB on the the fleet size is calculated                        " << std::endl;
			// std::cout << "[-round <bool>] rounding the distance to the nearest integer or not. It can be 0 (not rounding) or 1 (rounding). Defaults to 1. " << std::endl;
			// std::cout << "[-log <bool>] sets the verbose level of the algorithm log. It can be 0 or 1. Defaults to 1.                                     " << std::endl;
			// std::cout << std::endl;
			// std::cout << "Additional Arguments:                                                                                                           " << std::endl;
			// std::cout << "[-nbIterTraces <int>] Number of iterations between traces display during HGS execution. Defaults to 500                         " << std::endl;
			// std::cout << "[-nbGranular <int>] Granular search parameter, limits the number of moves in the RI local search. Defaults to 20                " << std::endl;
			// std::cout << "[-mu <int>] Minimum population size. Defaults to 25                                                                             " << std::endl;
			// std::cout << "[-lambda <int>] Number of solutions created before reaching the maximum population size (i.e., generation size). Defaults to 40 " << std::endl;
			// std::cout << "[-nbElite <int>] Number of elite individuals. Defaults to 5                                                                     " << std::endl;
			// std::cout << "[-nbClose <int>] Number of closest solutions/individuals considered when calculating diversity contribution. Defaults to 4      " << std::endl;
			// std::cout << "[-nbIterPenaltyManagement <int>] Number of iterations between penalty updates. Defaults to 100                                  " << std::endl;
			// std::cout << "[-targetFeasible <double>] target ratio of feasible individuals between penalty updates. Defaults to 0.2                        " << std::endl;
			// std::cout << "[-penaltyIncrease <double>] penalty increase if insufficient feasible individuals between penalty updates. Defaults to 1.2      " << std::endl;
			// std::cout << "[-penaltyDecrease <double>] penalty decrease if sufficient feasible individuals between penalty updates. Defaults to 0.85       " << std::endl;
			// std::cout << "--------------------------------------------------------------------------------------------------------------------------------" << std::endl;


		}
		
		void ComputeClusters(vector<item> packages, Coordinate warehouse, int numRiders, Bin b) {
							
			int nbVeh = numRiders;
			InstanceCVRPLIB cvrp(pathInstance, isRoundingInteger);
			Params params(cvrp.x_coords,cvrp.y_coords,cvrp.dist_mtx,cvrp.service_time,cvrp.demands,
						cvrp.vehicleCapacity,cvrp.durationLimit,nbVeh,cvrp.isDurationConstraint,verbose,ap);

			// Running HGS
			Genetic solver(params);
			solver.run();
			
			// Exporting the best solution
			if (solver.population.getBestFound() != NULL)
			{
				// clusters = // read from Solution file
				if (params.verbose) std::cout << "----- WRITING BEST SOLUTION IN : " << pathSolution << std::endl;
				solver.population.exportCVRPLibFormat(*solver.population.getBestFound(), pathSolution);
				solver.population.exportSearchProgress(pathSolution + ".PG.csv", pathInstance);
			}
		}

		float CalculateCost() {
			if (solver.population.getBestFound() != NULL)
			{
				cost = *solver.population.getBestFound().eval.penalizedCost ;	
			}
			
		}


	private:
		string pathInstance;		// Instance path
		string pathSolution;		// Solution path
		bool isRoundingInteger;
		bool verbose;
		AlgorithmParameters ap;
		
};

