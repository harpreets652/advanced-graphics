//
// Created by Harpreet Singh on 11/10/17.
//

#ifndef PARTICLES_RANDOMTEXTURE_H
#define PARTICLES_RANDOMTEXTURE_H

#include <random>
#define GLM_FORCE_RADIANS
#include "graphics_headers.h"

class RandomTexture {
public:
    RandomTexture();

    ~RandomTexture();

    bool initialize(int size);

    void enable(GLenum textureUnit);

private:
    GLuint randomTextureHandler;
};


#endif //PARTICLES_RANDOMTEXTURE_H
