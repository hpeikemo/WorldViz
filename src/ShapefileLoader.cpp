//
//  ShapefileLoader.cpp
//  WorldViz
//
//  Created by Hans Petter Eikemo on 19.08.10.
//

#include <cstring>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include "stdio.h"

#include "shapelib/shapefil.h"


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


pointList createPointList(SHPObject *obj, int rIndex) {
    int startIndex, stopIndex;
    pointList myList;    
    
    startIndex = obj->panPartStart[rIndex];
    if (rIndex+1 < obj->nParts) {
        stopIndex = obj->panPartStart[rIndex+1];
    } else {
        stopIndex = obj->nVertices;                    
    }
        
    for(int j=startIndex;j<stopIndex;j++) {
        shapePoint pt;
        pt.x=obj->padfX[j];
        pt.y=obj->padfY[j];
        myList.points.push_back(pt);
    }
    
    return myList;    
}


void loadShapefile( char *cpath, shapefileData *target ) {
    
    SHPHandle hSHP=SHPOpen(cpath, "rb" );
    if(hSHP == NULL) return;
        
    if(hSHP->nShapeType == SHPT_POLYGON) {
        
		for(int i=0;i<hSHP->nRecords;i++) {
            SHPObject *psShape = SHPReadObject(hSHP, i);
            
            for (int p=0;p<psShape->nParts;p++) {
                target->polygons.push_back( createPointList(psShape,p) );                
            }
        }
    }
        

    if(hSHP->nShapeType == SHPT_POINT) {
        for(int i=0;i<hSHP->nRecords;i++) {
            SHPObject *psShape = SHPReadObject(hSHP, i);
            shapePoint pt;
            pt.x= psShape->padfX[0];
            pt.y= psShape->padfY[0];
            target->points.push_back( pt );
        }
    }
    
    if(hSHP->nShapeType == SHPT_ARC) {
        
        for(int i=0;i<hSHP->nRecords;i++) {
            SHPObject *psShape = SHPReadObject(hSHP, i);
            
            for (int p=0;p<psShape->nParts;p++) {
                target->lines.push_back( createPointList(psShape,p) );                
            }
        }        
    }

    
}
 

