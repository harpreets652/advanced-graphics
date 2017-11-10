//
// Created by Harpreet Singh on 10/26/17.
//

#ifndef NORMALMAPPING_BILLBOARD_H
#define NORMALMAPPING_BILLBOARD_H

#include <iostream>
#include <string>
#include "graphics_headers.h"
#include "Texture.h"
#include "shader.h"

class Billboard {
public:
    Billboard();

    ~Billboard();

    bool initialize(std::string pTextureName);

    void render(glm::mat4 pProjectionViewMat, glm::vec3 pCameraPosition);

private:
    void setTextureUnit(unsigned int texUnit);

    GLuint vertexBuffer;
    Texture *billboardTexture;
    Shader *billboardShader;

    GLint projectViewMatrixHandler;
    GLint billboardSamplerHandler;
    GLint cameraPositionHandler;
};


#endif //NORMALMAPPING_BILLBOARD_H
