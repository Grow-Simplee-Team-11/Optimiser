#ifndef __DATASTRUCTURES__HEADER__
#define __DATASTRUCTURES__HEADER__

#include<iostream>
#include<fstream>
#include <bits/stdc++.h>

using namespace std;

struct Dimension_ {
    float length, width, height;
    Dimension_(){}
    Dimension_(float l, float w, float h)
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
    static constexpr double scale = 1e4;
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
        Position position;
        Dimension_ size;
        Coordinate coordinate;
        PolarCoordinate polarCoordinate;
        int time;
        float weight;
        float volume;
        item(){}
        item(float w, float h, float d, float lat, float lng)
        {
            size.width = w;
            size.height = h;
            size.length = d;
            coordinate.latitude = lat;
            coordinate.longitude = lng;
            weight = 0;
            volume = w*h*d;
        }
        void print(){
            cout<<coordinate.longitude<<" "<<coordinate.latitude<<"\n";
            // cout<<"( Latitude - "<<coordinate.latitude<<" Longitude - "<<coordinate.longitude<<" Size - ("<<size.width<<", "<<size.length<<", "<<size.height<<") )";
            return;
        }
        void printToFile(std::ofstream& out){
            out<<coordinate.longitude<<" "<<coordinate.latitude<<"\n";
            // out<<"( Latitude - "<<coordinate.latitude<<" Longitude - "<<coordinate.longitude<<" Size - ("<<size.width<<", "<<size.length<<", "<<size.height<<") )";
            return;
        }

        float getVolume() {
            return size.width * size.length * size.height;
        }
} ;

class Bin{
    public:
    Dimension_ size;
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