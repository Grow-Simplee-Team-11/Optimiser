/**
 * ClusteringInterface.hpp 
 * ~ Archisman Pathak
**/

#ifndef __CLUSTERING_INTERFACE__
#define __CLUSTERING_INTERFACE__

#include<vector>
#include<cstdint>
#include<string>
#include<iostream>
#include<fstream>
#define EUCLIDEAN 0
#define HAVERSINE 1
#include "../datastructures.hpp"
#define PI 3.14159265359
using namespace std;

class ClusteringInterface{
    public:
        const int SCALING_FACTOR=100; 
        bool method;
        ClusteringInterface(bool method) {
            this->method = method;
            cost = 0;
        }
        // store the computed cluster in clusters 
        virtual void ComputeClusters(vector<item>& packages, Coordinate warehouse, int numRiders, Bin b) = 0;
        // calculate the cost of the cluster computed 
        virtual void CalculateCost() = 0;

        void PrintClusters()
        {
            int32_t i=0;
            for(auto &cluster : clusters)
            {
                cout<<"Printing Cluster -- " << i++ <<endl;
                for(auto &it : cluster){
                    it.print();
                }
                
            }
            return;
        }

        void PrintClustersToFile(string filename)
        {
            std::ofstream out(filename, std::ios_base::app);
            int32_t i=0;
            for(auto &cluster : clusters)
            {
                out<<"Printing Cluster -- " << i++ <<" \n";
                for(auto &it : cluster){
                    it.printToFile(out);
                }
                
            }
            return;
        }

        vector<vector<item>> GetClusters(){
            return clusters;
        }

        float GetClusteringCost(){
            return cost;
        }

        void setClusters(vector<vector<item> > createdClusters) {
            clusters = createdClusters;
        }

        Coordinate getWarehouse(){
            return warehouse;
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
            // double PI = 3.14159265359;
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
        vector<vector<item>> clusters;
        float cost;
        Coordinate warehouse;
        Bin b;
        int numRiders;

};
#endif 