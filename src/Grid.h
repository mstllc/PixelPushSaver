//
//  Grid.h
//  PixelPushSaver
//
//  Created by Max Thorson on 5/31/17.
//
//

#pragma once

#include "cinder/Rand.h"
#include "Pixel.h"

using namespace ci;
using namespace ci::app;
using namespace std;

enum Direction {Up, Down, Left, Right};

class Grid {
public:
    
    Grid(int pixelSize, int gutterSize);
    
    void update();
    void draw();
    
    void resetPixelIterator();
    void addPixelAtSpace(ivec2 space);
    ivec2 getPosForSpace(ivec2 space);
    ivec2 getCenterSpace();
    
    int cols;
    int rows;
    
private:
    
    int pixelSize;
    int gutterSize;
    int vOffset;
    int hOffset;
    
    vector<Direction> directions {Up, Down, Left, Right};
    bool pixelCanMoveInDirection(Direction direction);
    bool pixelCanMoveUp();
    bool pixelCanMoveDown();
    bool pixelCanMoveLeft();
    bool pixelCanMoveRight();
    void movePixelInDirection(Direction direction);
    void movePixelUp();
    void movePixelDown();
    void movePixelLeft();
    void movePixelRight();
    
    bool pixelIsMoving = false;
    void onPixelMoveComplete();
    
    ivec2 attractor;
    void resetAttractor();
    
    vector<Pixel> pixels;
    vector<Pixel>::iterator pixelsIt;
    
    vector<vector<bool>> occupied;
};
