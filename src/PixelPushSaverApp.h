//
//  PixelPushSaverApp.h
//  PixelPushSaver
//
//  Created by Max Thorson on 5/31/17.
//
//

#pragma once

#include "cinder/app/AppScreenSaver.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "Grid.h"

using namespace ci;
using namespace ci::app;

class PixelPushSaverApp : public AppScreenSaver {
public:
    void setup() override;
    void update() override;
    void draw() override;
    
    Grid *grid;
};
