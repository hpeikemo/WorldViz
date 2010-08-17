 
//Program to Parse ESRI Shapefile and render it in OpenGL
//Author : Dhulipudi Durga Prasad
//Contact: dgplinux@yahoo.com
//Libraries Used: Shapelib ,OpenGL

#include <stdlib.h>
#include "stdio.h"
 
#include "shapelib/shapefil.h"

#include <vector>
using namespace std;
 
 
typedef struct MyPoint2D
{
 double dX;
 double dY;
 
}MyPoint2D;
 
//Holds Coordinates of Point Shapefile
vector<MyPoint2D> vPoints;
 
typedef struct MyLineString2D
{
 vector<MyPoint2D> vPointList;
 
}MyLineString2D;

//Holds Coordinates of Line Shapefile
vector<MyLineString2D> vLines;
 
 
typedef struct MyPolygon2D
{
 vector<MyPoint2D> vPointList;
 
}MyPolygon2D;
//Holds Coordinates of Polygon Shapefile 
vector<MyPolygon2D> vPolygons;
 
 

typedef struct SBoundingBox
{
 float fMaxX;
 float fMaxY;
 float fMinX;
 float fMinY;
 
}SBoundingBox;
//Bounding Box of Shapefile 
SBoundingBox sBoundingBox;
 
//Function to Open Shapefile and parse the info 
void OpenShapeFile(char* fileName)
{

    
    SHPHandle hSHP=SHPOpen(fileName, "rb" );
    
    
	//Read Bounding Box of Shapefile
    sBoundingBox.fMaxX=hSHP->adBoundsMax[0];
    sBoundingBox.fMaxY=hSHP->adBoundsMax[1];
    
    sBoundingBox.fMinX=hSHP->adBoundsMin[0];
    sBoundingBox.fMinY=hSHP->adBoundsMin[1];
        

    if(hSHP == NULL) return;

	//Point Shapefile
    if(hSHP->nShapeType == SHPT_POINT)
    {
        SHPObject *psShape;
        for(int i=0;i<hSHP->nRecords;i++)
        {
            psShape = SHPReadObject(hSHP, i);

            double fX = psShape->padfX[0];
            double fY = -psShape->padfY[0];

            //Plot these points
            MyPoint2D pt;
            pt.dX=fX;
            pt.dY=-fY;
            vPoints.push_back(pt);
        }
    }


	//Line Shapefile
    else if(hSHP->nShapeType == SHPT_ARC)
    {
        SHPObject *psShape;
	    for(int i=0;i<hSHP->nRecords;i++)
        {
            psShape = SHPReadObject(hSHP, i);
            vector<MyPoint2D> tempPointArray;

            for(int j=0;j<psShape->nVertices;j++)
            {
				double fX = psShape->padfX[j];
				double fY = psShape->padfY[j];
				MyPoint2D pt;
				pt.dX=fX;
				pt.dY=fY;
				tempPointArray.push_back(pt);
            }
           
			MyLineString2D linestring;
            linestring.vPointList=tempPointArray;
            vLines.push_back(linestring);
           
        }
    }

 	//Polygon Shapefile
    if(hSHP->nShapeType == SHPT_POLYGON)
    {
		SHPObject *psShape;
		for(int i=0;i<hSHP->nRecords;i++) {
            psShape = SHPReadObject(hSHP, i);

            for (int p=0;p<psShape->nParts;p++) {
                vector<MyPoint2D> tempPointArray;
                int pstart = psShape->panPartStart[p];
                int pend;
                if (p+1 < psShape->nParts) {
                    pend = psShape->panPartStart[p+1];
                } else {
                    pend = psShape->nVertices;                    
                }
                for(int j=pstart;j<pend;j++) {
                    double fX = psShape->padfX[j];
                    double fY = psShape->padfY[j];
                    MyPoint2D pt;
                    pt.dX=fX;
                    pt.dY=fY;
                    tempPointArray.push_back(pt);
                }
                MyPolygon2D polygon;
                polygon.vPointList=tempPointArray;
                vPolygons.push_back(polygon);
            }
        }
			      		            
        cout << "Created  "<< vPolygons.size() << endl;

        
    }
  
}
 