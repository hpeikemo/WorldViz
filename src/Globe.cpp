//
//  Globe.cpp
//  WorldViz
//
//  Created by Hans Petter Eikemo on 8/21/10.
//  Copyright (c) 2010 Apt. All rights reserved.
//


#include "Globe.h"
#include "cinder/Vector.h"

using namespace ci;

Vec3f coordinateToPoint(double x, double y) {   
    float r  = 2.0f;
    float p  = cos( (y/180.0f) *M_PI );
    float pz = cos( (x/180.0f) *M_PI ) * r *p;
    float px = sin( (x/180.0f) *M_PI ) * r *p;
    float py = sin( (y/180.0f) *M_PI ) * r;
    return Vec3f(px,py,pz);            
    //    return Vec3f(x/50,y/50,0);            
}

void drawShapefile(shapefileData *data) {
    
    //Render Polygon Shapefile	
	for(int i=0;i<data->polygons.size();i++) {        
        glBegin(GL_LINE_LOOP); //glBegin(GL_POLYGON);
		for(int j=0;j<data->polygons[i].points.size();j++) {    
            Vec3f p = coordinateToPoint(data->polygons[i].points[j].x, data->polygons[i].points[j].y);
            glVertex3f(p.x, p.y, p.z);
		}		
		glEnd();
	}
    
    //Render Line Shapefile
	for(int i=0;i<data->lines.size();i++) {		
		glBegin(GL_LINE_STRIP);
        
		for(int j=0;j<data->lines[i].points.size();j++) {
            Vec3f p = coordinateToPoint(data->lines[i].points[j].x, data->lines[i].points[j].y);
            glVertex3f(p.x, p.y, p.z);
        }		
        
		glEnd();
	}    
    
    //Render Point Shapefile
    glEnable(GL_POINT_SMOOTH) ;
	glPointSize(5.0);
    
    //    GLfloat color[] = { 1.0, 0.0, 0.0, 1.0 };
    //	glMaterialfv(GL_FRONT, GL_DIFFUSE, color );
    glBegin(GL_POINTS);
	for(int i=0;i<data->points.size();i++) {
        Vec3f p = coordinateToPoint(data->points[i].x,data->points[i].y);
        glVertex3f(p.x, p.y, p.z);
	}	
	glEnd();  
    
}




void Globe::update() {
    
    shapeDList = glGenLists(1);
    glNewList(shapeDList, GL_COMPILE);
    drawShapefile( &borderShapes );
    glEndList();
    
}

void Globe::render() {
    
    globeShader.bind();
    gl::drawSphere( Vec3f(0.0f, 0.0f, 0.0f), 1.999f, 40 );    
    globeShader.unbind();

    GLfloat mat_black[] = { 0.0, 0.0, 0.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_black);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_black);

    glCallList(shapeDList);
    
}