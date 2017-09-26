//
// Created by Harpreet Singh on 9/19/17.
//

#ifndef LIGHTING_TEXTURE_H
#define LIGHTING_TEXTURE_H


#include "graphics_headers.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

class Texture {
public:
    Texture();

    ~Texture();

    bool loadTexture(std::string &imageName);

    void enable();

    void disable();

private:
    GLuint textureHandler;
};


#endif //LIGHTING_TEXTURE_H
