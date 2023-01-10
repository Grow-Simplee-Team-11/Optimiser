#ifndef __DATASTRUCTURES__HEADER__
#define __DATASTRUCTURES__HEADER__

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

struct PolarCoordinate {
    float radius, angle;
};

struct item {
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
} ;

struct Bin{
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
};

#endif