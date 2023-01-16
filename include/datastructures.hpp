#ifndef __DATASTRUCTURES__HEADER__
#define __DATASTRUCTURES__HEADER__

#include <bits/stdc++.h>

using namespace std;

struct Dimension {
    float length, width, height;

    Dimension(){    }
    Dimension(float l, float w, float h)
    {
        length = l;
        width = w;
        height = h;
    }
};

struct Position {
    float x, y, z;
};

struct Coordinate {
    float longitude, latitude;
    Coordinate(){}
    Coordinate(float lat, float lng)
    {
        longitude = lng;
        latitude = lat;
    }
};

inline bool operator<(const Coordinate& a1, const Coordinate& a2)
{
    if (a1.longitude != a2.longitude) return a1.longitude < a2.longitude;
    if (a1.latitude != a2.latitude) return a1.latitude < a2.latitude;
    return false;
} 

struct PolarCoordinate {
    float radius, angle;
};

class item {
    public:
        int id;
        Dimension size;
        Position position;
        Coordinate coordinate;
        PolarCoordinate polarCoordinate;
        float weight;
        item(){}
        item(float w, float h, float d, float lat, float lng)
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
            out<<"( Latitude - "<<coordinate.latitude<<" Longitude - "<<coordinate.longitude<<" Size - ("<<size.width<<", "<<size.length<<", "<<size.height<<") )";
            return;
        }
        float getVolume() {
            return size.width * size.length * size.height;
        }
} ;

class Bin{
    public:
    Dimension size;
    float capacity;
    Bin(){}
    Bin(float w,float l, float h){
        size.width = w;
        size.length = l;
        size.height = h;
        capacity = 1000;
    }

    Bin(float w, float l, float h, float c){
        size.width = w;
        size.length = l;
        size.height = h;
        capacity = c;
    }

    float getCapacity(){
        return capacity;
    }

    float getVolume(){
        return size.width * size.length * size.height * 0.85;
    }

};

#endif