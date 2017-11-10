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

    bool init(const std::string &pBaseDirectory,
              const std::string &pPosXTexture,
              const std::string &pNegXTexture,
              const std::string &pPosYTexture,
              const std::string &pNegYTexture,
              const std::string &pPosZTexture,
              const std::string &pNegZTexture);

    void render(glm::mat4 pViewProjectionMatrix);

private:
    Shader *skyBoxShader;
    Object *skyBoxMesh;
    CubeMapTexture *skyBoxTexture;

    GLint mvpHandler;
    GLint skyBoxSamplerHandler;
};


#endif //SKYBOX_SKYBOX_H
