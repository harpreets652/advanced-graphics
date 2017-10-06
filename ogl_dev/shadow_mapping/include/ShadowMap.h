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

    bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

    void BindForWriting();

    void BindForReading(GLenum TextureUnit);

private:
    GLuint m_fbo;
    GLuint m_shadowMap;
};


#endif //SHADOWMAPPING_SHADOWMAP_H
