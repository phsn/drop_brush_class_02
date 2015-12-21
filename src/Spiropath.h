#pragma once

#include "ofMain.h"

class Spiropath {
    
public:
    Spiropath();
    ~Spiropath();
    
    void setup();
    void update();
    
    ofVec3f getPos(float percentage);
    ofVec3f getVel(float percentage);
    
private:
    
    float globalRadius;
    
};
