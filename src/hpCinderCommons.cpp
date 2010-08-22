//
//  hpCinderCommons.cpp
//  WorldViz
//
//  Created by Hans Petter Eikemo on 8/22/10.
//  Copyright (c) 2010 Apt. All rights reserved.
//

#include "hpCinderCommons.h"
#include "cinder/DataSource.h"

using namespace cinder;

namespace hp { namespace ciCommons { 


    gl::GlslProg compileShader(DataSourceRef vertexShader, DataSourceRef fragmentShader,DataSourceRef geometryShader) {
        gl::GlslProg shader;    
        try {
            shader = gl::GlslProg( vertexShader, fragmentShader, geometryShader );
        } catch( ci::gl::GlslProgCompileExc &exc ) {
            std::cout << "Shader compile error: " << std::endl;
            std::cout << exc.what();
        } catch( ... ) {
            std::cout << "Unable to load shader" << std::endl;
        }
        return shader;
    }

    

} }