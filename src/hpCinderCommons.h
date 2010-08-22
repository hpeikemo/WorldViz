//
//  hpCinderCommons.h
//  WorldViz
//
//  Created by Hans Petter Eikemo on 8/22/10.
//

#pragma once
#include "cinder/gl/GlslProg.h"
#include "cinder/DataSource.h"

using namespace cinder;

namespace hp { namespace ciCommons {
        
        
    gl::GlslProg compileShader(DataSourceRef vertexShader, DataSourceRef fragmentShader = DataSourceRef(),DataSourceRef geometryShader = DataSourceRef());



} }
