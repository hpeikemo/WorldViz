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

#include "Resources.h"
#include "shapefile.h"
#include "Globe.h"

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
        
    Globe           world;
};





void WorldWizApp::prepareSettings( Settings* settings ) {
    settings->setWindowSize( 1280 , 720 );
    settings->setFrameRate( 30.0 );
}


void loadShapes(string path, shapefileData *targetShape) {
    char *cpath;
    cpath = new char[path.length() + 1];
    strcpy(cpath, path.c_str());
    loadShapefile( cpath, targetShape );
}


void WorldWizApp::setup() {    
    
    loadShapes( getResourcePath(RES_GLOBE_SHAPE), &world.borderShapes );
    world.update();
            
    try {
        world.globeShader = gl::GlslProg( loadResource( RES_GHOST_VERT ), loadResource( RES_GHOST_FRAG ) );
    } catch( ci::gl::GlslProgCompileExc &exc ) {
        std::cout << "Shader compile error: " << std::endl;
        std::cout << exc.what();
    } catch( ... ) {
        std::cout << "Unable to load shader" << std::endl;
    }

            

    
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
    glShadeModel(GL_FLAT); //glShadeModel(GL_SMOOTH);
    
    world.render();
        
    
    glPopMatrix();

    
}


CINDER_APP_BASIC( WorldWizApp, RendererGl )