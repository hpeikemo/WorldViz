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
#include "GLRenderSHP.cpp"

#include <cstring>
#include <list>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GLUT/glut.h>

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


void BasicParticleApp::prepareSettings( Settings* settings ) {
    settings->setWindowSize( 1280 , 720 );
    settings->setFrameRate( 60.0f );
}

void loadShapefile(string path) {
    char *cpath;
    cpath = new char[path.length() + 1];
    strcpy(cpath, path.c_str());
    OpenShapeFile( cpath );
}


void BasicParticleApp::setup() {    
    
  //loadShapefile( getResourcePath("world.shp") );
  //loadShapefile( getResourcePath("places.shp") );    
  //loadShapefile( getResourcePath("TM_WORLD_BORDERS-0.3.shp") );
    loadShapefile( getResourcePath("TM_WORLD_BORDERS_SIMPL-0.3.shp") );
    
    
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
    gluPerspective (50.0, (float)width/(float)height, 1.5, 50);
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
}


float orbitDegrees = 0.0f;

void BasicParticleApp::draw() {
    
    GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
    GLfloat lm_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

    GLfloat torus_diffuse[] = { 0.7, 0.7, 0.0, 1.0 };
    GLfloat cube_diffuse[] = { 0.0, 0.7, 0.7, 1.0 };
    GLfloat sphere_diffuse[] = { 0.7, 0.0, 0.7, 1.0 };
    GLfloat octa_diffuse[] = { 0.7, 0.4, 0.4, 1.0 };

    
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 50.0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel (GL_FLAT); //glShadeModel(GL_SMOOTH);

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearAccum(0.0, 0.0, 0.0, 0.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    glLoadIdentity();
    
    orbitDegrees += 1.0f;
    glRotatef(orbitDegrees, 0.f, 1.f, 0.f);/* orbit the Y axis */

    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, octa_diffuse);
    //gl::drawSphere( Vec3f(0.0f, 0.0f, 0.0f), 1.95f, 40 );

    
    //glDisable(GL_LIGHTING);
    glColor3f(1.0,0.0, 0.0);

    //Render Polygon Shapefile	
	for(int i=0;i<vPolygons.size();i++)
	{
		glBegin(GL_LINE_LOOP);
		for(int j=0;j<vPolygons[i].vPointList.size();j++) {    
            Vec3f p = coordinateToPoint(vPolygons[i].vPointList[j].dX, vPolygons[i].vPointList[j].dY);
            glVertex3f(p.x, p.y, p.z);
		}
		
		glEnd();
	}
    /*
    for(int i=0;i<18;i++) {
                
        glBegin(GL_LINE_STRIP);
        for(int j=0;j<40;j++) {    
            Vec3f p = coordinateToPoint(i*10,(90/40)*j );
            glVertex3f(p.x, p.y, p.z);
        }    
        glEnd();
    
    }
    */

    glPopMatrix();
    
    /*
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    glClear (GL_COLOR_BUFFER_BIT);
	glColor3f (0.0, 0.0, 1.0);
	glLoadIdentity ();
	
	//Render Point Shapefile
	glColor3f (0.0, 0.0, 1.0);
	glEnable(GL_POINT_SMOOTH) ;
	glPointSize(5.0);
	glBegin(GL_POINTS);
	
	for(int i=0;i<vPoints.size();i++)
	{
		glVertex2f(vPoints[i].dX,vPoints[i].dY);
	}
	
	glEnd();
	
	//Render Line Shapefile
	glColor3f (0.0, 1.0, 0.0);
	for(int i=0;i<vLines.size();i++)
	{
		
		glBegin(GL_LINE_STRIP);
		for(int j=0;j<vLines[i].vPointList.size();j++)
		{
            glVertex2f(vLines[i].vPointList[j].dX,vLines[i].vPointList[j].dY);
            
		}
		
		glEnd();
	}
	
    
    glFlush();    
	*/
}


CINDER_APP_BASIC( BasicParticleApp, RendererGl )