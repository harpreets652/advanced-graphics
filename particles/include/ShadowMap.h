//
// Created by Harpreet Singh on 10/6/17.
//

#ifndef SHADOWMAPPING_SHADOWMAP_H
#define SHADOWMAPPING_SHADOWMAP_H

#include "graphics_headers.h"
#include <SDL2/SDL.h>
#include "shader.h"


class ShadowMap {
public:
    ShadowMap();

    ~ShadowMap();

    bool initialize(Shader &renderShader, int windowWidth, int windowHeight);

    void bindForWriting();

    void enableProgram();

    void setMVP(glm::mat4 pMVP);

    void setTextureUnit(unsigned int texUnit);

    void bindForReading(GLenum TextureUnit);

private:
    bool init(int windowWidth, int windowHeight);

    GLuint fboHandler;
    GLuint shadowMapHandler;
    GLint mvpHandler;
    GLint shadowSamplerHandler;
    Shader *shadowShader;
};


#endif //SHADOWMAPPING_SHADOWMAP_H
