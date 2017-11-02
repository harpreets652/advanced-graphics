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

    bool loadTextures(const std::string &pBaseDirectory,
                      const std::string &pPosXTexture,
                      const std::string &pNegXTexture,
                      const std::string &pPosYTexture,
                      const std::string &pNegYTexture,
                      const std::string &pPosZTexture,
                      const std::string &pNegZTexture);

    void enable(GLenum textureUnit);

private:
    GLuint textureHandler;
    std::vector<std::string> cubeTextures;
};


#endif //SKYBOX_CUBEMAPTEXTURE_H
