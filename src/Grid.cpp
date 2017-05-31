//
//  Grid.cpp
//  PixelPushSaver
//
//  Created by Max Thorson on 5/31/17.
//
//

#include "Grid.h"

#define MOVE_DURATION 0.1
#define PRINCE_PURPLE Color(138.0 / 255.0, 0.0 / 255.0, 172.0 / 255.0)

Grid::Grid(int _pixelSize, int _gutterSize) {
    pixelSize = _pixelSize;
    gutterSize = _gutterSize;
    
    cols = floor((getWindowWidth() - gutterSize) / (pixelSize + gutterSize));
    if (cols % 2 == 0) cols -= 1;
    rows = floor((getWindowHeight() - gutterSize) / (pixelSize + gutterSize));
    if (rows % 2 == 0) rows -= 1;
    
    hOffset = ((getWindowWidth() - gutterSize) - ((pixelSize + gutterSize) * cols)) / 2;
    vOffset = ((getWindowHeight() - gutterSize) - ((pixelSize + gutterSize) * rows)) / 2;
    
    occupied.resize(cols, vector<bool>(rows, false));
    
    Rand::randomize();
    attractor = ivec2(randInt(cols), randInt(rows));
    
    resetPixelIterator();
}

void Grid::update() {
    if (!pixelIsMoving) {
        if (pixelsIt == pixels.end()) {
            resetPixelIterator();
        }
        
        vec2 diff = attractor - ivec2(pixelsIt->col, pixelsIt->row);
        
        random_shuffle(directions.begin(), directions.end());
        
        for (vector<Direction>::iterator direction = directions.begin(); direction != directions.end(); ++direction) {
            if (diff.x > 0 && *direction == Right) {
                rotate(directions.begin(), direction, direction + 1);
            } else if (diff.x < 0 && *direction == Left) {
                rotate(directions.begin(), direction, direction + 1);
            } else if (diff.y > 0 && *direction == Down) {
                rotate(directions.begin(), direction, direction + 1);
            } else if (diff.y < 0 && *direction == Up) {
                rotate(directions.begin(), direction, direction + 1);
            }
        }
        
        for (vector<Direction>::iterator direction = directions.begin(); direction != directions.end(); ++direction) {
            if (pixelCanMoveInDirection(*direction)) {
                pixelIsMoving = true;
                movePixelInDirection(*direction);
                break;
            }
        }
        
        ++pixelsIt;
    }
}

void Grid::draw() {
    ivec2 attractorPos = getPosForSpace(attractor);
    Rectf attractorSpace = Rectf(attractorPos.x - 0.5, attractorPos.y - 0.5, attractorPos.x + 0.5, attractorPos.y + 0.5);
    attractorSpace.scaleCentered(pixelSize);
    gl::color(PRINCE_PURPLE);
    gl::drawStrokedRect(attractorSpace, 2.0);
    
    gl::color(Color::white());
    for (vector<Pixel>::iterator pixel = pixels.begin(); pixel != pixels.end(); ++pixel) {
        pixel->draw();
    }
}

void Grid::resetPixelIterator() {
    pixelsIt = pixels.begin();
}

void Grid::addPixelAtSpace(ivec2 space) {
    Pixel p = Pixel(pixelSize, space, getPosForSpace(space));
    if (space == attractor) {
        p.color = PRINCE_PURPLE;
        p.hasOccupiedAttractor = true;
    }
    pixels.push_back(p);
    occupied[space.x][space.y] = true;
}

ivec2 Grid::getPosForSpace(ivec2 space) {
    int x = hOffset + ((pixelSize + gutterSize) * (space.x + 1)) - (pixelSize / 2);
    int y = vOffset + ((pixelSize + gutterSize) * (space.y + 1)) - (pixelSize / 2);
    return ivec2(x, y);
}

ivec2 Grid::getCenterSpace() {
    int col = floor(cols / 2.0);
    int row = floor(rows / 2.0);
    return vec2(col, row);
}

bool Grid::pixelCanMoveInDirection(Direction direction) {
    if (direction == Up) {
        return pixelCanMoveUp();
    } else if (direction == Down) {
        return pixelCanMoveDown();
    } else if (direction == Left) {
        return pixelCanMoveLeft();
    } else {
        return pixelCanMoveRight();
    }
}

bool Grid::pixelCanMoveUp() {
    if (pixelsIt->row - 1 < 0) {
        return false;
    } else {
        return !occupied[pixelsIt->col][pixelsIt->row - 1];
    }
}

bool Grid::pixelCanMoveDown() {
    if (pixelsIt->row + 1 > rows - 1) {
        return false;
    } else {
        return !occupied[pixelsIt->col][pixelsIt->row + 1];
    }
}

bool Grid::pixelCanMoveLeft() {
    if (pixelsIt->col - 1 < 0) {
        return false;
    } else {
        return !occupied[pixelsIt->col - 1][pixelsIt->row];
    }
}

bool Grid::pixelCanMoveRight() {
    if (pixelsIt->col + 1 > cols - 1) {
        return false;
    } else {
        return !occupied[pixelsIt->col + 1][pixelsIt->row];
    }
}

void Grid::movePixelInDirection(Direction direction) {
    if (direction == Up) {
        movePixelUp();
    } else if (direction == Down) {
        movePixelDown();
    } else if (direction == Left) {
        movePixelLeft();
    } else {
        movePixelRight();
    }
    if (ivec2(pixelsIt->col, pixelsIt->row) == attractor) {
        pixelsIt->color = PRINCE_PURPLE;
        pixelsIt->hasOccupiedAttractor = true;
    }
}

void Grid::movePixelUp() {
    occupied[pixelsIt->col][pixelsIt->row] = false;
    pixelsIt->row -= 1;
    occupied[pixelsIt->col][pixelsIt->row] = true;
    timeline().apply(&pixelsIt->rect, pixelsIt->rect.value() - ivec2(0, pixelSize + gutterSize), MOVE_DURATION, EaseInQuart())
    .finishFn(bind(&Grid::onPixelMoveComplete, this));
}

void Grid::movePixelDown() {
    occupied[pixelsIt->col][pixelsIt->row] = false;
    pixelsIt->row += 1;
    occupied[pixelsIt->col][pixelsIt->row] = true;
    timeline().apply(&pixelsIt->rect, pixelsIt->rect.value() + ivec2(0, pixelSize + gutterSize), MOVE_DURATION, EaseInQuart())
    .finishFn(bind(&Grid::onPixelMoveComplete, this));
}

void Grid::movePixelLeft() {
    occupied[pixelsIt->col][pixelsIt->row] = false;
    pixelsIt->col -= 1;
    occupied[pixelsIt->col][pixelsIt->row] = true;
    timeline().apply(&pixelsIt->rect, pixelsIt->rect.value() - ivec2(pixelSize + gutterSize, 0), MOVE_DURATION, EaseInQuart())
    .finishFn(bind(&Grid::onPixelMoveComplete, this));
}

void Grid::movePixelRight() {
    occupied[pixelsIt->col][pixelsIt->row] = false;
    pixelsIt->col += 1;
    occupied[pixelsIt->col][pixelsIt->row] = true;
    timeline().apply(&pixelsIt->rect, pixelsIt->rect.value() + ivec2(pixelSize + gutterSize, 0), MOVE_DURATION, EaseInQuart())
    .finishFn(bind(&Grid::onPixelMoveComplete, this));
}

void Grid::onPixelMoveComplete() {
    bool allOccupied = true;
    for (vector<Pixel>::iterator pixel = pixels.begin(); pixel != pixels.end(); ++pixel) {
        if (!pixel->hasOccupiedAttractor) {
            allOccupied = false;
            break;
        }
    }
    
    if (allOccupied) {
        resetAttractor();
    }
    
    pixelIsMoving = false;
}

void Grid::resetAttractor() {
    Rand::randomize();
    attractor = ivec2(randInt(cols), randInt(rows));
    
    for (vector<Pixel>::iterator pixel = pixels.begin(); pixel != pixels.end(); ++pixel) {
        if (ivec2(pixel->col, pixel->row) == attractor) {
            pixel->color = PRINCE_PURPLE;
            pixel->hasOccupiedAttractor = true;
        } else {
            pixel->color = Color::white();
            pixel->hasOccupiedAttractor = false;
        }
    }
}
