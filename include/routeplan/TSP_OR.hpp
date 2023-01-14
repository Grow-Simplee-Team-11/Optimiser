// TSP approximation using google OR tools
#ifndef __TSP_OR__
#define __TSP_OR__

#include"../interface/RoutePlanInterface.hpp"
#include "ortools/constraint_solver/routing.h"
#include "ortools/constraint_solver/routing_enums.pb.h"
#include "ortools/constraint_solver/routing_index_manager.h"
#include "ortools/constraint_solver/routing_parameters.h"

using namespace operations_research;
class TSP_OR : public RoutePlanInterface{

    public : 
        TSP_OR(){}
        void PlanRoute(vector<item>& cluster, Coordinate warehouse);
        void CalculateCost();
        public : 
        // convert longitude to X
        double getX(double lon);

        // convert latitude to Y
        double getY(double lat);

        // Compute distances between every pair of points
        void ComputeEuclideanDistanceMatrix(std::vector<item>& cluster);

        // Convert the Google OR-Tool based route to vector<item> Path
        void savePath(vector<item>&clusters, const RoutingIndexManager& manager,
                   const RoutingModel& routing, const Assignment& solution);

        Coordinate getWarehouse(){
            return warehouse;
        }
    private:
        
        std::vector<std::vector<int64_t>> distances;
        const int num_vehicles=1;
        Coordinate warehouse;
        const RoutingIndexManager::NodeIndex depot{0};
};

#endif