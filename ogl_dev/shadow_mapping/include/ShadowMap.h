//
// Created by Harpreet Singh on 10/6/17.
//

#ifndef SHADOWMAPPING_SHADOWMAP_H
#define SHADOWMAPPING_SHADOWMAP_H

#include "graphics_headers.h"
#include <SDL2/SDL.h>


class ShadowMap {
public:
    ShadowMap();

    ~ShadowMap();

    bool init(int WindowWidth, int WindowHeight);

    void bindForWriting();

    void bindForReading(GLenum TextureUnit);

private:
    GLuint fboHandler;
    GLuint shadowMapHandler;
};


#endif //SHADOWMAPPING_SHADOWMAP_H
