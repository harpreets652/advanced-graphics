//
// Created by Harpreet Singh on 10/26/17.
//

#include "CubeMapTexture.h"

static const GLenum glCubeMapTextureTypes[6] = {GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                                                GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                                                GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                                                GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                                                GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                                                GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};

CubeMapTexture::CubeMapTexture() {
    textureHandler = 0;
    cubeTextures.clear();
}

CubeMapTexture::~CubeMapTexture() {
    if (textureHandler != 0) {
        glDeleteTextures(1, &textureHandler);
    }
}

bool CubeMapTexture::loadTextures(const std::string &pBaseDirectory,
                                  const std::string &pPosXTexture,
                                  const std::string &pNegXTexture,
                                  const std::string &pPosYTexture,
                                  const std::string &pNegYTexture,
                                  const std::string &pPosZTexture,
                                  const std::string &pNegZTexture) {
    
    cubeTextures.push_back(pBaseDirectory + pPosXTexture);
    cubeTextures.push_back(pBaseDirectory + pNegXTexture);
    cubeTextures.push_back(pBaseDirectory + pPosYTexture);
    cubeTextures.push_back(pBaseDirectory + pNegYTexture);
    cubeTextures.push_back(pBaseDirectory + pPosZTexture);
    cubeTextures.push_back(pBaseDirectory + pNegZTexture);

    glGenTextures(1, &textureHandler);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureHandler);

    for (int i = 0; i < cubeTextures.size(); i++) {
        SDL_Surface *textureImage = IMG_Load(cubeTextures[i].c_str());

        if (textureImage == nullptr) {
            std::cerr << "failed to load image: " << IMG_GetError() << std::endl;
            return false;
        }

        glTexImage2D(glCubeMapTextureTypes[i],
                     0,
                     textureImage->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB,
                     (GLsizei) textureImage->w,
                     (GLsizei) textureImage->h,
                     0,
                     textureImage->format->BytesPerPixel == 4 ? GL_RGBA : GL_RGB,
                     GL_UNSIGNED_BYTE,
                     textureImage->pixels);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        SDL_FreeSurface(textureImage);
    }

    return true;
}

void CubeMapTexture::enable(GLenum textureUnit) {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureHandler);
}