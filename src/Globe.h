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

class Globe {
    public:
    
        void update();
        void render();
        
        shapefileData   borderShapes;
        ci::gl::GlslProg    globeShader;
    
    private:
        
        GLuint shapeDList;
    
};


