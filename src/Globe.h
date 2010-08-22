//
//  Globe.h
//  WorldViz
//
//  Created by Hans Petter Eikemo on 8/21/10.
//

#pragma once
#include "cinder/gl/GlslProg.h"

#include <OpenGL/glu.h>
#include "shapefile.h"
#include "cinder/Vector.h"

class Globe {
    public:
    
        void update();
        void render();
        ci::Vec3f toVec3f(double x,double y, double a=0.0f);
        
        shapefileData   borderShapes;
        ci::gl::GlslProg    globeShader;
    
    private:
        
        GLuint shapeDList;
    
};


