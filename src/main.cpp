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
#include "cinder/DataSource.h"


#include <cstring>
#include <list>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <OpenGL/glu.h>

#include "Resources.h"
#include "shapefile.h"
#include "Globe.h"
#include "hpCinderCommons.h"

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
    void    load();
    
    Globe           world;
};


void WorldWizApp::prepareSettings( Settings* settings ) {
    settings->setWindowSize( 1280 , 720 );
    settings->setFrameRate( 30.0 );
}


void WorldWizApp::load() {    
    world.globeShader = hp::ciCommons::compileShader( loadResource( RES_GHOST_VERT ), loadResource( RES_GHOST_FRAG ) );
    
    //Debug:
    /*
    world.globeShader = hp::ciCommons::compileShader( 
                                                     cinder::DataSourcePath::createRef( "/Users/hpe/Dropbox/Work/C++/WorldViz/shaders/freshnel.vs" ),
                                                     cinder::DataSourcePath::createRef( "/Users/hpe/Dropbox/Work/C++/WorldViz/shaders/freshnel.fs" )); */
}

void WorldWizApp::setup() {    
    
    loadShapefile( getResourcePath(RES_GLOBE_SHAPE), &world.borderShapes );
    world.update();
                
    load();
    
}


void WorldWizApp::mouseDown( MouseEvent event ) {
}

void WorldWizApp::keyDown( KeyEvent event ) {
    
	if( event.getChar() == 'f' )
		setFullScreen( ! isFullScreen() );
    
    if( event.getChar() == 'r' )
		load();

}

void WorldWizApp::update() {

}


float screenAspect = 1.0f;

void WorldWizApp::resize( int width, int height ) {
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    screenAspect = (float)width/(float)height;        
}


float orbitDegrees = 0.0f;

void WorldWizApp::draw() {
            
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClearAccum(0.0, 0.0, 0.0, 0.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (23.0, screenAspect, 0.5, 50);
    
    
    //Vec3f cpos = Vec3f(cos(orbitDegrees)*12.0f ,0.0f,sin(orbitDegrees)*12.0f);
    //gluLookAt(cpos.x,cpos.y,cpos.z, 0,0,0, 0,1,0);        
   // cpos.x = 0;
   // cpos.y = 0;
   // cpos.z = 0;
    
    gluLookAt(0,0,0, -1,0,0, 0,1,0);    
    
   // world.globeShader.uniform("CAMERA_POSITION",cpos);
    
    glMatrixMode(GL_MODELVIEW);
    
    glPushMatrix();
    glLoadIdentity();
    
    glPushMatrix();

    glLoadIdentity();
    GLfloat light_position[] = { -10.0, 4.0, 0.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    GLfloat lm_ambient[] = { 0.5, 0.5, 0.5, 1.0 };    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lm_ambient);    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glPopMatrix();
    
    glTranslatef( -15., 0, 0 );
    orbitDegrees += 0.3f;
    glRotatef(orbitDegrees, 0.f, 1.f, 0.5f);
    

    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    //glEnable(GL_POLYGON_SMOOTH);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);
    
    glShadeModel(GL_FLAT);//glShadeModel(GL_SMOOTH);
    
    glLineWidth(1.5);
    
    world.render();        
    
    glPopMatrix();

    
}


CINDER_APP_BASIC( WorldWizApp, RendererGl )