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

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distribution(0.0,1.0);

    double coneAngleRad = 20.0 * (3.14/180.0);
    for (unsigned int i = 0; i < size; i++) {
        //https://stackoverflow.com/questions/38997302/create-random-unit-vector-inside-a-defined-conical-region/39003745#39003745
        double randVal = distribution(gen);
        double phi = randVal * 2 * 3.1415;
        double z = randVal * (1 - glm::cos(coneAngleRad)) + glm::cos(coneAngleRad);
        double x = std::sqrt(1 - std::pow(z, 2)) * glm::cos(phi);
        double y = std::sqrt(1 - std::pow(z, 2)) * glm::sin(phi);

        pRandomData[i].x = (float)x;
        pRandomData[i].y = (float)z;
        pRandomData[i].z = (float)y;
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