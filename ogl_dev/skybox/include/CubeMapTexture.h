//
// Created by Harpreet Singh on 10/26/17.
//

#ifndef SKYBOX_CUBEMAPTEXTURE_H
#define SKYBOX_CUBEMAPTEXTURE_H

#include "graphics_headers.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <string>
#include <vector>

class CubeMapTexture {
public:
    CubeMapTexture();

    ~CubeMapTexture();

    bool loadTextures(std::string &pBaseDirectory,
                      std::string &pPosXTexture,
                      std::string &pNegXTexture,
                      std::string &pPosYTexture,
                      std::string &pNegYTexture,
                      std::string &pPosZTexture,
                      std::string &pNegZTexture);

    void enable(GLenum textureUnit);

private:
    GLuint textureHandler;
    std::vector<std::string> cubeTextures;
};


#endif //SKYBOX_CUBEMAPTEXTURE_H
