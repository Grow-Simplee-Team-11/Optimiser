#ifndef __DATASTRUCTURES__HEADER__
#define __DATASTRUCTURES__HEADER__
#include<cmath>
double R = 6371;
struct Dimension {
    double length, width, height;

    Dimension(){    }
    Dimension(double l, double w, double h)
    {
        length = l;
        width = w;
        height = h;
    }
    double getVol(){
        return length*width*height;
    }
};

struct Position {
    double x, y, z;
};

struct Coordinate {
    double longitude, latitude;
    Coordinate(){}
    Coordinate(double lat, double lng)
    {
        longitude = lng;
        latitude = lat;
    }
};
double toRadians(const double degree){
    double one_deg = (M_PI) / 180;
    return (one_deg * degree);
}
double Dist(Coordinate &c1,Coordinate &c2){
    double lat1 = toRadians(c1.latitude);
    double long1 = toRadians(c1.longitude);
    double lat2 = toRadians(c2.latitude);
    double long2 = toRadians(c2.longitude);

    double dlong = long2 - long1;
    double dlat = lat2 - lat1;

    double ans = pow(sin(dlat / 2), 2) +
                          cos(lat1) * cos(lat2) *
                          pow(sin(dlong / 2), 2);
 
    ans = 2 * asin(sqrt(ans));
    ans = ans * R;
    return ans;
}
struct PolarCoordinate {
    double radius, angle;
};

struct item {
    Dimension size;
    Position position;
    Coordinate coordinate;
    PolarCoordinate polarCoordinate;
    double weight;
    item(){}
    item(double w, double h, double d, double lat, double lng)
    {
        size.width = w;
        size.height = h;
        size.length = d;
        coordinate.latitude = lat;
        coordinate.longitude = lng;
        weight = 0;
    }
    void print(){
        cout<<"( Latitude - "<<coordinate.latitude<<" Longitude - "<<coordinate.longitude<<" Size - ("<<size.width<<", "<<size.length<<", "<<size.height<<") )";
        return;
    }
    void printToFile(std::ofstream& out){
        cout<<"( Latitude - "<<coordinate.latitude<<" Longitude - "<<coordinate.longitude<<" Size - ("<<size.width<<", "<<size.length<<", "<<size.height<<") )";
        return;
    }
} ;

struct Bin{
    Dimension size;
    double capacity;
    Bin(){}
    Bin(double w,double l, double h){
        size.width = w;
        size.length = l;
        size.height = h;
        capacity = 1000;
    }

    Bin(double w, double l, double h, double c){
        size.width = w;
        size.length = l;
        size.height = h;
        capacity = c;
    }
    double getVol(){
        size.getVol();
    }
};

#endif