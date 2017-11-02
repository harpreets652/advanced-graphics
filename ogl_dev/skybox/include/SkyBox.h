//
// Created by Harpreet Singh on 11/2/17.
//

#ifndef SKYBOX_SKYBOX_H
#define SKYBOX_SKYBOX_H

#include "shader.h"
#include "object.h"
#include "CubeMapTexture.h"

#include "graphics_headers.h"

class SkyBox {
public:
    SkyBox() {}

    ~SkyBox() {}

    bool init(std::string &pBaseDirectory,
              std::string &pPosXTexture,
              std::string &pNegXTexture,
              std::string &pPosYTexture,
              std::string &pNegYTexture,
              std::string &pPosZTexture,
              std::string &pNegZTexture);

    void render(glm::mat4 pViewProjectionMatrix);

private:
    Shader *skyBoxShader;
    Object *skyBoxMesh;//Note~ new renderSkyBox method
    CubeMapTexture *skyBoxTexture;

    GLint mvpHandler;
    GLint skyBoxSamplerHandler;
};


#endif //SKYBOX_SKYBOX_H
