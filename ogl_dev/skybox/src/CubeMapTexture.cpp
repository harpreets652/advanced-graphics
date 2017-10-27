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
    cubeTextures.reserve(6);
}

CubeMapTexture::~CubeMapTexture() {
    if (textureHandler != 0) {
        glDeleteTextures(1, &textureHandler);
    }
}

bool CubeMapTexture::loadTextures(std::string &pBaseDirectory,
                                  std::string &pPosXTexture,
                                  std::string &pNegXTexture,
                                  std::string &pPosYTexture,
                                  std::string &pNegYTexture,
                                  std::string &pPosZTexture,
                                  std::string &pNegZTexture) {
    cubeTextures[0] = pBaseDirectory + pPosXTexture;
    cubeTextures[1] = pBaseDirectory + pNegXTexture;
    cubeTextures[2] = pBaseDirectory + pPosYTexture;
    cubeTextures[3] = pBaseDirectory + pNegYTexture;
    cubeTextures[4] = pBaseDirectory + pPosZTexture;
    cubeTextures[5] = pBaseDirectory + pNegZTexture;

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