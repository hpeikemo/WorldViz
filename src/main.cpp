#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "cinder/Vector.h"
#include "cinder/Perlin.h"
#include "cinder/Color.h"
#include "cinder/gl/gl.h"
//#include "shapelib/shapefil.h"
#include "stdio.h"

#include <list>
#include <vector>
#include <cstring>

#include "GLRenderSHP.cpp"

using std::list;

using namespace std;
using namespace ci;
using namespace ci::app;


class BasicParticleApp : public AppBasic {
public:	
	void	setup();
	void	mouseDown( MouseEvent event );
	void	keyDown( KeyEvent event );		
	void	update();
	void	draw();	
    
};



void loadShapefile(string path) {
    char *cpath;
    cpath = new char[path.length() + 1];
    strcpy(cpath, path.c_str());
    OpenShapeFile( cpath );
}


void BasicParticleApp::setup() {    
    
    loadShapefile( getResourcePath("strassen.shp") );
    loadShapefile( getResourcePath("poi.shp") );
    loadShapefile( getResourcePath("gruenflaechen.shp") );
    
}


void BasicParticleApp::mouseDown( MouseEvent event ) {
}

void BasicParticleApp::keyDown( KeyEvent event ) {
	if( event.getChar() == 'f' )
		setFullScreen( ! isFullScreen() );
}

void BasicParticleApp::update() {

}

void BasicParticleApp::draw() {
	
    int w = getWindowWidth();
    int h = getWindowHeight();
    
    if(h<=0) h=1 ;
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	//Assign Bounding Box Coordinates of Shapefile to glOrtho()
	glOrtho(sBoundingBox.fMinX, sBoundingBox.fMaxX,sBoundingBox.fMinY,sBoundingBox.fMaxY,-1,1);
	glMatrixMode(GL_MODELVIEW);

    
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
	
	//Render Polygon Shapefile
	glColor3f(1.0,0.0, 0.0);
	for(int i=0;i<vPolygons.size();i++)
	{
		glBegin(GL_LINE_LOOP);
		for(int j=0;j<vPolygons[i].vPointList.size();j++) {
			glVertex2f(vPolygons[i].vPointList[j].dX,vPolygons[i].vPointList[j].dY);
		}
		
		glEnd();
	}
    
    glFlush();

    
	
}


CINDER_APP_BASIC( BasicParticleApp, RendererGl )