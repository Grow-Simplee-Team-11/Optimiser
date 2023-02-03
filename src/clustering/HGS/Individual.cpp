#include "../../../include/clustering/HGS/Individual.h" 

/**
 * @brief evaluate cost of the solution using a penalty function, with terms for exceeding distance, 
 * duration and capacity constraints
 * 
 * 
 * @param params problem-specific data including location coordinates and penalty weights
 */
void Individual::evaluateCompleteCost(const Params & params)
{
	eval = EvalIndiv();
	int zeroCount = 0;
	for (int r = 0; r < params.nbVehicles; r++)
	{	
		
		if (!chromR[r].empty())
		{	
			// if(chromR[r].size() > 50){
			// 	std::cout << chromR[r].size() << std::endl;
			// 	// more_than_50 = true;
			// }
			// else std::cout << "Less than 50 :  " << chromR[r].size() << std::endl;
				
			
			double distance = params.timeCost[0][chromR[r][0]];
			double load = params.cli[chromR[r][0]].demand;
			double service = params.cli[chromR[r][0]].serviceDuration;
			predecessors[chromR[r][0]] = 0;
			int countOfIncrease = 0;
			// std::cout << chromR[r].size() << ' ' << params.cli[chromR[r][0]].serviceDuration << std::endl; 
			for (int i = 1; i < (int)chromR[r].size(); i++)
			{	
				distance += params.timeCost[chromR[r][i-1]][chromR[r][i]];
				float timeReach = (distance)/params.averageSpeed + service ; //Add starting time in this
				double diff = timeReach-params.timeExpectation[chromR[r][i]];
				countOfIncrease += (diff>0?diff:0);
				load += params.cli[chromR[r][i]].demand;
				service += params.cli[chromR[r][i]].serviceDuration;
				predecessors[chromR[r][i]] = chromR[r][i-1];
				successors[chromR[r][i-1]] = chromR[r][i];
			}
			successors[chromR[r][chromR[r].size()-1]] = 0;
			distance += params.timeCost[chromR[r][chromR[r].size()-1]][0];
			eval.distance += distance + countOfIncrease*params.penaltyEDD;
			eval.nbRoutes++;
			if (load > params.vehicleCapacity) eval.capacityExcess += load - params.vehicleCapacity;
			if (distance / params.averageSpeed + service > params.durationLimit) eval.durationExcess += distance / params.averageSpeed + service - params.durationLimit;
			// if(eval.capacityExcess > MY_EPSILON){
			// 	std::cout << "INFEASIABLE CAPACITY" << std::endl;
			// }
			// else if(eval.durationExcess > MY_EPSILON){
			// 	std::cout << "INFEASIABLE DURATION" << " Duration Limit " << params.durationLimit <<  " Distance : " << distance << " Distance / avg Speed : " <<distance/params.averageSpeed <<  " service time : " << service << std::endl;
			// }
		}
		else{
			zeroCount++;
		}
	}
	std::cout << "Zero Count : " << zeroCount << ' ' << params.penaltyCapacity << ' ' << params.penaltyDuration << std::endl;

	// Add to final cost.
	eval.penalizedCost = eval.distance + eval.capacityExcess*params.penaltyCapacity + eval.durationExcess*params.penaltyDuration;
	// Check for feasibility
	eval.isFeasible = (eval.capacityExcess < MY_EPSILON && eval.durationExcess < MY_EPSILON) ;
	if(eval.capacityExcess > MY_EPSILON){
		std::cout << "INFEASIABLE CAPACITY" << std::endl;
	}
	else if(eval.durationExcess > MY_EPSILON){
		std::cout << "INFEASIABLE DURATION" << std::endl;
	}
	else if(eval.isFeasible)
		std::cout << "***************************************************FOUND FEASIBLE***********************************" << std::endl;
	// if(eval.isFeasible)
	// 	std::cout << "***************************************************FOUND FEASIBLE***********************************" << std::endl;
	// else if(pkg_more_than_25){
	// 	std::cout << "INFEASIABLE PKG MORE THAN 25" << std::endl;
	// }
}

/**
 * @brief Construct a new Individual:: Individual object
 * chromR: in actual solution format - with a vector of vectors, each corresponding to one rider;
 * 			the nested vectors contain route to be travelled by each rider
 * chromT: assigns an index to all the clients
 * 
 * @param params problem-specific data including location coordinates and penalty weights
 */
Individual::Individual(Params & params)
{
	successors = std::vector <int>(params.nbClients + 1);
	predecessors = std::vector <int>(params.nbClients + 1);
	chromR = std::vector < std::vector <int> >(params.nbVehicles);
	chromT = std::vector <int>(params.nbClients);
	// timeMatrix = params.timeMatrix;
	for (int i = 0; i < params.nbClients; i++) chromT[i] = i + 1;
	std::shuffle(chromT.begin(), chromT.end(), params.ran);
	eval.penalizedCost = 1.e30;	
}

/**
 * @brief Construct a new Individual:: Individual object
 * 
 * @param params problem-specific data including location coordinates and penalty weights
 * @param fileName input file containing coordinates, demands of all clients, and vehicle capacity, distance & service time constraints
 */
Individual::Individual(Params & params, std::string fileName) : Individual(params)
{
	double readCost;
	chromT.clear();
	std::ifstream inputFile(fileName);
	if (inputFile.is_open())
	{
		std::string inputString;
		inputFile >> inputString;
		// Loops in the input file as long as the first line keyword is "Route"
		for (int r = 0; inputString == "Route"; r++)
		{
			inputFile >> inputString;
			getline(inputFile, inputString);
			std::stringstream ss(inputString);
			int inputCustomer;
			while (ss >> inputCustomer) // Loops as long as there is an integer to read in this route
			{
				chromT.push_back(inputCustomer);
				chromR[r].push_back(inputCustomer);
			}
			inputFile >> inputString;
		}
		if (inputString == "Cost") inputFile >> readCost;
		else throw std::string("Unexpected token in input solution");

		// Some safety checks and printouts
		evaluateCompleteCost(params);
		if ((int)chromT.size() != params.nbClients) throw std::string("Input solution does not contain the correct number of clients");
		if (!eval.isFeasible) throw std::string("Input solution is infeasible");
		if (eval.penalizedCost != readCost)throw std::string("Input solution has a different cost than announced in the file");
		if (params.verbose) std::cout << "----- INPUT SOLUTION HAS BEEN SUCCESSFULLY READ WITH COST " << eval.penalizedCost << std::endl;
	}
	else 
		throw std::string("Impossible to open solution file provided in input in : " + fileName);
}