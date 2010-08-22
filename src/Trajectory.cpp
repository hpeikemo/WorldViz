//
//  Trajectory.cpp
//  WorldViz
//
//  Created by Hans Petter Eikemo on 8/22/10.
//  Copyright (c) 2010 Apt. All rights reserved.
//

#include "Trajectory.h"
#include "cinder/Vector.h"
#include "Globe.h"
#include <math.h>

using namespace ci;

void Trajectory::render(Globe *world) {
    
    glBegin(GL_LINE_STRIP);
    GLfloat mat_light[] = { 0.8, 0.8, 0.8, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_light);
    
    Vec2f start (originPoint.x , originPoint.y);
    Vec2f end   (targetPoint.x , targetPoint.y);
    
    
    Vec2f delta = end-start;
    
    Vec2f rStart = start / 180.0f * M_PI;
    Vec2f rEnd = end / 180.0f * M_PI;

    Vec2f rDelta = rEnd-rStart;
    
    for(int i=0,l=30;i<=l;++i) {
                        
        //Vec2f pos = start+ delta/l * i;                
        //Vec3f p = world->toVec3f(pos.x,pos.y, sin((double)i/(double)l*M_PI)*apex);
        
        Vec2f rPos = rStart + rDelta/l * i;
        
        rPos = rPos/M_PI * 180.0f;
        
        Vec3f p = world->toVec3f(rPos.x,rPos.y, .1f);        
        glVertex3f(p.x, p.y, p.z);
                
    }
    
    glEnd();
    
    
};