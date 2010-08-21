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

#include <OpenGL/glu.h>

#include "ShapefileLoader.cpp"
#include "Resources.h"

using std::list;

using namespace std;
using namespace ci;
using namespace ci::app;


class WorldWizApp : public AppBasic {
public:	
    void    prepareSettings( Settings* settings );
	void	setup();
	void	mouseDown( MouseEvent event );
	void	keyDown( KeyEvent event );		
    void    resize( int width, int height );
	void	update();
	void	draw();	
    

};



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



shapefileData   shapes;
gl::GlslProg	mShader;
GLuint          shapeDList;



void WorldWizApp::prepareSettings( Settings* settings ) {
    settings->setWindowSize( 1280 , 720 );
    settings->setFrameRate( 30.0 );
}


void loadShapes(string path) {
    char *cpath;
    cpath = new char[path.length() + 1];
    strcpy(cpath, path.c_str());
    loadShapefile( cpath, &shapes );
}


void WorldWizApp::setup() {    
    
    loadShapes( getResourcePath(RES_GLOBE_SHAPE) );
    

    
    try {
        mShader = gl::GlslProg( loadResource( RES_GHOST_VERT ), loadResource( RES_GHOST_FRAG ) );
    }
    catch( ci::gl::GlslProgCompileExc &exc ) {
        std::cout << "Shader compile error: " << std::endl;
        std::cout << exc.what();
    }
    catch( ... ) {
        std::cout << "Unable to load shader" << std::endl;
    }

    
    
    shapeDList = glGenLists(1);
    glNewList(shapeDList, GL_COMPILE);
    drawShapefile( &shapes );
    glEndList();

    
}


void WorldWizApp::mouseDown( MouseEvent event ) {
}

void WorldWizApp::keyDown( KeyEvent event ) {
    
	if( event.getChar() == 'f' )
		setFullScreen( ! isFullScreen() );
}

void WorldWizApp::update() {

}

void WorldWizApp::resize( int width, int height ) {
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (23.0, (float)width/(float)height, 1.5, 50);
    gluLookAt(8,2,8, 0,0,0, 0,1,0);    
    glMatrixMode(GL_MODELVIEW);    
}



float orbitDegrees = 0.0f;

void WorldWizApp::draw() {
            
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearAccum(0.0, 0.0, 0.0, 0.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    glLoadIdentity();
    
    orbitDegrees += 0.2f;
    glRotatef(orbitDegrees, 0.f, 1.f, 0.5f);

    GLfloat light_position[] = { 8.0, 8.0, 8.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    GLfloat lm_ambient[] = { 0.5, 0.5, 0.5, 1.0 };    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    //glShadeModel(GL_SMOOTH);

    
    GLfloat mat_black[] = { 0.0, 0.0, 0.0, 0.7 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_black);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_black);
    
    mShader.bind();
    gl::drawSphere( Vec3f(0.0f, 0.0f, 0.0f), 1.99f, 40 );    
    mShader.unbind();
    
    GLfloat torus_diffuse[] = { 0.7, 0.7, 0.6, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, torus_diffuse);

        
//    drawShapefile( &shapes );
    glCallList(shapeDList);
        
    
    glPopMatrix();

    
}


CINDER_APP_BASIC( WorldWizApp, RendererGl )