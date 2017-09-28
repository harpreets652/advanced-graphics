//
// Created by Harpreet Singh on 9/19/17.
//

#include "Texture.h"

Texture::Texture() {
    textureHandler = 0;
}

Texture::~Texture() {
    if (textureHandler != 0) {
        glDeleteTextures(1, &textureHandler);
    }
}

bool Texture::loadTexture(std::string &imageName) {
    SDL_Surface *textureImage = IMG_Load(imageName.c_str());

    if (textureImage == nullptr) {
        std::cerr << "failed to load image: " << IMG_GetError() << std::endl;
        return false;
    }

    glGenTextures(1, &textureHandler);
    glBindTexture(GL_TEXTURE_2D, textureHandler);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //or GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //or GL_NEAREST

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 textureImage->format->BitsPerPixel == 4 ? GL_RGBA : GL_RGB,
                 textureImage->w,
                 textureImage->h,
                 0,
                 textureImage->format->BitsPerPixel == 4 ? GL_RGBA : GL_RGB,
                 GL_UNSIGNED_BYTE,
                 textureImage->pixels);

    return true;
}

//TextureUnit can be GL_TEXTURE0
void Texture::enable(GLenum textureUnit) {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureHandler);
}

void Texture::disable() {
    glDisable(GL_TEXTURE_2D);
}