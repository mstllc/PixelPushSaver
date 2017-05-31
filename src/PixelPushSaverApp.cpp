//
//  PixelPushSaverApp.cpp
//  PixelPushSaver
//
//  Created by Max Thorson on 5/31/17.
//
//

#include "PixelPushSaverApp.h"

#define PIXEL_SIZE 30
#define GUTTER_SIZE 10
#define PIXEL_GRID 5

void PixelPushSaverApp::setup() {
    grid = new Grid(PIXEL_SIZE, GUTTER_SIZE);
    
    ivec2 centerSpace = grid->getCenterSpace();
    int row = centerSpace.y - floor(PIXEL_GRID / 2.0);
    int col = centerSpace.x - floor(PIXEL_GRID / 2.0);
    for (int r = row; r < (row + PIXEL_GRID); r++) {
        for (int c = col; c < (col + PIXEL_GRID); c++) {
            grid->addPixelAtSpace(ivec2(c, r));
        }
    }
    
    grid->resetPixelIterator();
}

void PixelPushSaverApp::update() {
	grid->update();
}

void PixelPushSaverApp::draw() {
    gl::clear(Color::black());
    grid->draw();
}

CINDER_APP_SCREENSAVER(PixelPushSaverApp, RendererGl)
