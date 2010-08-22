//
//  shapefile.h
//  WorldViz
//
//  Created by Hans Petter Eikemo on 8/21/10.
//

#pragma once
#include "shapelib/shapefil.h"
#include <vector>
#include <cstring>


using namespace std;

struct shapePoint {
    double x;
    double y;    
};

struct pointList {
    vector<shapePoint> points;    
};

struct shapefileData {
    
    vector<shapePoint>  points;
    vector<pointList>   lines;
    vector<pointList>   polygons;
    
};

void loadShapefile( char *cpath, shapefileData *target );
void loadShapefile( const string &path, shapefileData *target );
