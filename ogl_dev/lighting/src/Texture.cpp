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
    ILuint devID;
    ilGenImages(1, &devID);
    ilBindImage(devID);
    bool success = ilLoadImage(imageName.c_str());

    if (!success) {
        std::cout << "IMAGE COULD NOT BE LOADED : " << imageName << std::endl;
        return false;
    }

    glGenTextures(1, &textureHandler);
    glBindTexture(GL_TEXTURE_2D, textureHandler);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //or GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //or GL_NEAREST

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 ilGetInteger(IL_IMAGE_BPP),
                 ilGetInteger(IL_IMAGE_WIDTH),
                 ilGetInteger(IL_IMAGE_HEIGHT),
                 0,
                 ilGetInteger(IL_IMAGE_FORMAT),
                 GL_UNSIGNED_BYTE,
                 ilGetData());

    ilDeleteImages(1, &devID);

    return true;
}

//bool Texture::loadTexture(std::string &imageName) {
//
//    int maxSize = 0;
//    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
//
//    SDL_Surface *textureImage = IMG_Load(imageName.c_str());
//
//    if (textureImage == nullptr) {
//        std::cerr << "failed to load image: " << IMG_GetError() << std::endl;
//        return false;
//    }
//
//    glGenTextures(1, &textureHandler);
//    glBindTexture(GL_TEXTURE_2D, textureHandler);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //or GL_NEAREST
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //or GL_NEAREST
//
//    glTexImage2D(GL_TEXTURE_2D,
//                 0,
//                 GL_RGB,
//                 textureImage->w,
//                 textureImage->h,
//                 0,
//                 GL_RGB,
//                 GL_UNSIGNED_BYTE,
//                 textureImage->pixels);
//
//    return true;
//}

void Texture::enable() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureHandler);
}

void Texture::disable() {
    glDisable(GL_TEXTURE_2D);
}