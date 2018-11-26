//
// Created by Harpreet Singh on 9/23/17.
//

#include "TextureManager.h"

TextureManager *TextureManager::instance = nullptr;

TextureManager *TextureManager::getInstance() {
    if (!instance) {
        instance = new TextureManager;
    }

    return instance;
}

TextureManager::TextureManager() {
    samplerHandler = 0;
}

bool TextureManager::initHandlers(Shader &shaderManager) {
    samplerHandler = shaderManager.getUniformLocation("gSampler");

    return samplerHandler != INVALID_UNIFORM_LOCATION;
}

bool TextureManager::addTexture(const std::string textName, std::string imageFileName) {
    auto *texture = new Texture;
    bool status = texture->loadTexture(imageFileName);

    if (status) {
        textures.insert({textName, texture});
    }

    return status;
}

void TextureManager::setTextureUnit(unsigned int samplerIndex) {
    if (samplerHandler != 0) {
        glUniform1i(samplerHandler, samplerIndex);
    }
}

void TextureManager::enableTexture(std::string textName, GLenum textureUnit) {
    if (textures.find(textName) != textures.end()) {
        textures[textName]->enable(textureUnit);
    }
}

void TextureManager::disableTexture(std::string textName) {
    if (textures.find(textName) != textures.end()) {
        textures[textName]->disable();
    }
}

