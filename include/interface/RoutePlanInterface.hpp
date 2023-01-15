/**
 * RoutePlanInterface.hpp
 * ~Rohit Raj
**/
#ifndef __ROUTEPLANPACK_INTERFACE__
#define __ROUTEPLANPACK_INTERFACE__

#include<vector>
#include<fstream>
#include<iostream>
#include"../datastructures.hpp"
using namespace std;

class RoutePlanInterface{
    public:
        RoutePlanInterface() {
            cost = 0;
        }
        // store the path planned for all clusters  
        virtual void PlanRoute(vector<item> cluster, Coordinate warehouse, Bin b) = 0;
        // calculate the cost of the path computed 
        virtual float CalculateCost() = 0; 

        void PrintRoutes()
        {
            int32_t i=0;
            cout<<"Printing the routes for the cluster -- " << endl;
            cout<<"Warehouse -> ";
            for(auto &it : plannedPath){
                it.print(); cout<<" -> ";
            }
            cout<<"-> Warehouse"<<endl;
            return;
        }

        void PrintRoutesToFile(string filename)
        {
            std::ofstream out(filename);
            int32_t i=0;
            out<<"The Routes for the cluster -- " << endl;
            out<<"Warehouse -> ";
            for(auto &it : plannedPath){
                it.printToFile(out); out<<" -> ";
            }
            out<<"-> Warehouse"<<endl;
            return;
        }

        vector<item> GetPaths(){
            return plannedPath;
        }

        float GetPathPlanningCost(){
            return cost;
        }

    private:
        vector<item> plannedPath;
        float cost;

};

#endif 