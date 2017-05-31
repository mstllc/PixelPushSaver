//
//  Pixel.h
//  PixelPushSaver
//
//  Created by Max Thorson on 5/31/17.
//
//

#pragma once

#include "cinder/app/AppScreenSaver.h"
#include "cinder/gl/gl.h"
#include "cinder/Timeline.h"

using namespace ci;

class Pixel {
public:
    
    Pixel(int size, ivec2 space, vec2 pos);
    
    void setup();
    void update();
    void draw();
    
    int col;
    int row;
    
    bool hasOccupiedAttractor = false;
    
    Color color;
    
    Anim<Rectf> rect;
    
};
