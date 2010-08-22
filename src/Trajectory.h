//
//  Trajectory.h
//  WorldViz
//
//  Created by Hans Petter Eikemo on 8/22/10.
//

#pragma once
#include "cinder/Vector.h"
#include "shapefile.h"
#include "Globe.h"

class Trajectory {
public:
    
    void render(Globe *world);

    shapePoint  originPoint,
                targetPoint;
    
    float apex;
    
    
};