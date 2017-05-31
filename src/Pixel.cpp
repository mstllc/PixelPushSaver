//
//  Pixel.cpp
//  PixelPushSaver
//
//  Created by Max Thorson on 5/31/17.
//
//

#include "Pixel.h"

Pixel::Pixel(int size, ivec2 space, vec2 pos) {
    rect.value().set(0, 0, size, size);
    rect.value().offsetCenterTo(pos);
    col = space.x;
    row = space.y;
    
    color = Color::white();
}

void Pixel::setup() {
    
}

void Pixel::update() {
    
}

void Pixel::draw() {
    gl::color(color);
    gl::drawSolidRect(rect);
}
