//
// Created by Harpreet Singh on 11/2/17.
//

#include "SkyBox.h"

bool SkyBox::init(std::string &pBaseDirectory,
                  std::string &pPosXTexture,
                  std::string &pNegXTexture,
                  std::string &pPosYTexture,
                  std::string &pNegYTexture,
                  std::string &pPosZTexture,
                  std::string &pNegZTexture) {

    //initialize shader
    skyBoxShader = new Shader();
    if (!skyBoxShader->initialize()) {
        std::cout << "Sky Box shader program failed to initialize" << std::endl;
        return false;
    }

    //add vertex, geometry, and fragment shader
    if (!skyBoxShader->addShaderFromFile("../shaders/skyBox.vert", GL_VERTEX_SHADER)) {
        std::cout << "Vertex shader failed to initialize" << std::endl;
        return false;
    }

    if (!skyBoxShader->addShaderFromFile("../shaders/skyBox.frag", GL_FRAGMENT_SHADER)) {
        std::cout << "Fragment Shader failed to Initialize" << std::endl;
        return false;
    }

    if (!skyBoxShader->finalize()) {
        std::cout << "Failed to finalize billboard shader program" << std::endl;
        return false;
    }

    mvpHandler = skyBoxShader->getUniformLocation("mvpMatrix");
    skyBoxSamplerHandler = skyBoxShader->getUniformLocation("gSkyBoxTexture");

    //initialize cube map texture
    skyBoxTexture = new CubeMapTexture();
    bool textureLoaded = skyBoxTexture->loadTextures(pBaseDirectory,
                                                     pPosXTexture,
                                                     pNegXTexture,
                                                     pPosYTexture,
                                                     pNegYTexture,
                                                     pPosZTexture,
                                                     pNegZTexture);

    if (!textureLoaded) {
        std::cout << "Unable to load sky box texture" << std::endl;
        return false;
    }

    //initialize cube
    skyBoxMesh = new Object("../objects/sphere.obj");
    glm::scale(skyBoxMesh->GetModel(), glm::vec3(21, 21, 21));

    return true;
}

void SkyBox::render(glm::mat4 pProjectionViewMatrix) {
    skyBoxShader->enable();

    GLint oldCullFaceMode, oldDepthFuncMode;
    glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);

    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);

    glm::mat4 mvp = pProjectionViewMatrix * skyBoxMesh->GetModel();

    glUniformMatrix4fv(mvpHandler, 1, GL_FALSE, glm::value_ptr(mvp));
    glUniform1i(skyBoxSamplerHandler, 3);

    skyBoxTexture->enable(GL_TEXTURE3);

    skyBoxMesh->SimpleRender();

    glCullFace((GLenum) oldCullFaceMode);
    glDepthFunc((GLenum) oldDepthFuncMode);
}
