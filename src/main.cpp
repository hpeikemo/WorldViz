#include "cinder/Color.h"
#include "cinder/ImageIo.h"
#include "cinder/MayaCamUI.h"
#include "cinder/ObjLoader.h"
#include "cinder/Perlin.h"
#include "cinder/Rand.h"
#include "cinder/Rect.h"
#include "cinder/Sphere.h"
#include "cinder/Vector.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/gl.h"


#include <cstring>
#include <list>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

//#include <GLUT/glut.h>
#include <OpenGL/glu.h>

#include "ShapefileLoader.cpp"

using std::list;

using namespace std;
using namespace ci;
using namespace ci::app;


class BasicParticleApp : public AppBasic {
public:	
    void    prepareSettings( Settings* settings );
	void	setup();
	void	mouseDown( MouseEvent event );
	void	keyDown( KeyEvent event );		
    void    resize( int width, int height );
	void	update();
	void	draw();	
    

};

shapefileData shapes;

void BasicParticleApp::prepareSettings( Settings* settings ) {
    settings->setWindowSize( 1280 , 720 );
    settings->setFrameRate( 30.0 );
}


void loadShapes(string path) {
    char *cpath;
    cpath = new char[path.length() + 1];
    strcpy(cpath, path.c_str());
    
//    OpenShapeFile( cpath );
    loadShapefile( cpath, &shapes );
}


void BasicParticleApp::setup() {    
    
  //loadShapefile( getResourcePath("world.shp") );
    loadShapes( getResourcePath("places.shp") );    
  //loadShapefile( getResourcePath("TM_WORLD_BORDERS-0.3.shp") );
    loadShapes( getResourcePath("TM_WORLD_BORDERS_SIMPL-0.3.shp") );
        
}


void BasicParticleApp::mouseDown( MouseEvent event ) {
}

void BasicParticleApp::keyDown( KeyEvent event ) {
	if( event.getChar() == 'f' )
		setFullScreen( ! isFullScreen() );
}

void BasicParticleApp::update() {

}

void BasicParticleApp::resize( int width, int height ) {
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (23.0, (float)width/(float)height, 1.5, 50);
    gluLookAt(8,2,8, 0,0,0, 0,1,0);    
    glMatrixMode(GL_MODELVIEW);    
}

Vec3f coordinateToPoint(double x, double y) {   
    float r  = 2.0f;
    float p  = cos( (y/180.0f) *M_PI );
    float pz = cos( (x/180.0f) *M_PI ) * r *p;
    float px = sin( (x/180.0f) *M_PI ) * r *p;
    float py = sin( (y/180.0f) *M_PI ) * r;
    return Vec3f(px,py,pz);            
//    return Vec3f(x/50,y/50,0);            
}


float orbitDegrees = 0.0f;

void BasicParticleApp::draw() {
    
    GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_black[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_position[] = { 8.0, 8.0, 8.0, 1.0 };
    GLfloat lm_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

    GLfloat torus_diffuse[] = { 0.7, 0.7, 0.0, 1.0 };
    
    
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearAccum(0.0, 0.0, 0.0, 0.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    glLoadIdentity();
    
    orbitDegrees += 0.2f;
    glRotatef(orbitDegrees, 0.f, 1.f, 0.f);/* orbit the Y axis */

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 120.0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel (GL_FLAT);
    //glShadeModel(GL_SMOOTH);

    
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_black);
//    gl::drawSphere( Vec3f(0.0f, 0.0f, 0.0f), 1.99f, 40 );

    glMaterialfv(GL_FRONT, GL_DIFFUSE, torus_diffuse);

    //glDisable(GL_LIGHTING);
    glColor3f(1.0,0.0, 0.0);

    //Render Polygon Shapefile	
	for(int i=0;i<shapes.polygons.size();i++) {        
        glBegin(GL_LINE_LOOP); //glBegin(GL_POLYGON);
		for(int j=0;j<shapes.polygons[i].points.size();j++) {    
            Vec3f p = coordinateToPoint(shapes.polygons[i].points[j].x, shapes.polygons[i].points[j].y);
            glVertex3f(p.x, p.y, p.z);
		}		
		glEnd();
	}
    
    //Render Line Shapefile
	glColor3f (0.0, 1.0, 0.0);
	for(int i=0;i<shapes.lines.size();i++) {		
		glBegin(GL_LINE_STRIP);

		for(int j=0;j<shapes.lines[i].points.size();j++) {
            //shapePoint pnt = shapes.lines[i].points[j];
            Vec3f p = coordinateToPoint(shapes.lines[i].points[j].x, shapes.lines[i].points[j].y);
            glVertex3f(p.x, p.y, p.z);
        }		

		glEnd();
	}    

    //Render Point Shapefile
    glEnable(GL_POINT_SMOOTH) ;
	glPointSize(5.0);
    
    GLfloat color[] = { 1.0, 0.0, 0.0, 1.0 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color );
    glBegin(GL_POINTS);	
	for(int i=0;i<shapes.points.size();i++) {
        Vec3f p = coordinateToPoint(shapes.points[i].x,shapes.points[i].y);
        glVertex3f(p.x, p.y, p.z);
	}	
	glEnd();    
    
        
    
    glPopMatrix();

    
}


CINDER_APP_BASIC( BasicParticleApp, RendererGl )