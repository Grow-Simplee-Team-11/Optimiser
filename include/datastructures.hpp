#ifndef __DATASTRUCTURES__HEADER__
#define __DATASTRUCTURES__HEADER__
#include<cmath>
#include<iostream>
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

struct PolarCoordinate {
    double radius, angle;
};

struct item {
    Dimension size;
    Position position;
    Coordinate coordinate;
    PolarCoordinate polarCoordinate;
    double weight;
    double volume;
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
        std::cout<<"( Latitude - "<<coordinate.latitude<<" Longitude - "<<coordinate.longitude<<" Size - ("<<size.width<<", "<<size.length<<", "<<size.height<<") )";
        return;
    }
    void printToFile(std::ofstream& out){
        std::cout<<"( Latitude - "<<coordinate.latitude<<" Longitude - "<<coordinate.longitude<<" Size - ("<<size.width<<", "<<size.length<<", "<<size.height<<") )";
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
        return size.getVol();
    }
};

#endif