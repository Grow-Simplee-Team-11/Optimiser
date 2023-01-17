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
#define EUCLIDEAN 0
#define HAVERSINE 1
using namespace std;

class RoutePlanInterface{
    public:
    const int SCALING_FACTOR=100; 
    bool method;
        RoutePlanInterface(bool method) {
            this -> method = method;
            cost = 0;
        }
        // store the path planned for all clusters  
        virtual void PlanRoute(vector<item>& cluster, Coordinate warehouse) = 0;
        // calculate the cost of the path computed 
        virtual void CalculateCost() = 0; 

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
            std::ofstream out(filename, std::ios_base::app);
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
        double Dist(struct Coordinate c1,struct Coordinate c2){
            if(method == EUCLIDEAN)
                return euclidean(c1,c2);
            else if(method == HAVERSINE) 
                return haversine(c1,c2)*SCALING_FACTOR;
        }
        double euclidean(struct Coordinate c1,struct Coordinate c2){
            return sqrt((c1.latitude - c2.latitude)*(c1.latitude - c2.latitude) + (c1.longitude - c2.longitude)*(c1.longitude - c2.longitude));
        }
        double haversine(struct Coordinate c1,struct Coordinate c2){
            double lat_1_deg = c1.latitude;
            double lon_1_deg = c1.longitude;
            double lat_2_deg = c2.latitude;
            double lon_2_deg = c2.longitude;
            double PI = 3.14159265359;
            double lat_1_rad, lon_1_rad, lat_2_rad, lon_2_rad;
            lat_1_rad = lat_1_deg * (PI / 180);
            lon_1_rad = lon_1_deg * (PI / 180);
            lat_2_rad = lat_2_deg * (PI / 180);
            lon_2_rad = lon_2_deg * (PI / 180);
            double delta_lat, delta_lon;
            delta_lat = lat_1_rad - lat_2_rad;
            delta_lon = lon_1_rad - lon_2_rad;

            // Calculate sin^2 (delta / 2) for both lat and long
            double sdlat = pow(sin(delta_lat / 2), 2);
            double sdlon = pow(sin(delta_lon / 2), 2);

            // Radius of the Earth (approximate)
            const double radius_earth_miles = 3963;
            const double radius_earth_km = 6378;

            // http://en.wikipedia/org/wiki/Haversine_formula
            // d=2r*asin(sqrt(sin^2((lat1-lat2)/2)+cos(l1)cos(l2)sin^2((lon2-lon1)/2)))
            //  if t = sqrt(sin^2((lat1-lat2)/2)+cos(l1)cos(l2)sin^2((lon2-lon1)/2)
            //  -> d = 2 * radius_earth * asin (t)	
            double t = sqrt(sdlat + (cos(lat_1_rad) * cos(lat_2_rad) * sdlon));
            double distance_miles = 2 * radius_earth_miles * asin(t);
            double distance_km = 2 * radius_earth_km * asin(t);
            return distance_km;
        }


    protected:
        vector<item> plannedPath;
        float cost;

};

#endif 