//
// Created by Harpreet Singh on 11/10/17.
//

#include "RandomTexture.h"

RandomTexture::RandomTexture() {
    randomTextureHandler = 0;
}

RandomTexture::~RandomTexture() {
    if (randomTextureHandler != 0) {
        glDeleteTextures(1, &randomTextureHandler);
    }
}

bool RandomTexture::initialize(int size) {
    auto *pRandomData = new glm::vec3[size];
    for (unsigned int i = 0; i < size; i++) {
        pRandomData[i].x = float(rand() % 100);
        pRandomData[i].y = float(rand() % 100);
        pRandomData[i].z = float(rand() % 100);
    }

    glGenTextures(1, &randomTextureHandler);
    glBindTexture(GL_TEXTURE_1D, randomTextureHandler);
    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, size, 0, GL_RGB, GL_FLOAT, pRandomData);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    delete[] pRandomData;

    return true;
}

void RandomTexture::enable(GLenum textureUnit) {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_1D, randomTextureHandler);
}