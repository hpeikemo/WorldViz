#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "cinder/Vector.h"
#include "cinder/Perlin.h"
#include "cinder/Color.h"
#include "cinder/gl/gl.h"

#include <list>
using std::list;

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

void BasicParticleApp::setup() {
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
	// clear out the screen
	gl::clear( Color( 0.1f, 0.1f, 0.1f ) );
	
}


CINDER_APP_BASIC( BasicParticleApp, RendererGl )