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

    if (samplerHandler == INVALID_UNIFORM_LOCATION) {
        return false;
    }

    return true;
}

bool TextureManager::addTexture(const std::string textName, std::string imageFileName) {
    Texture texture;
    bool status = texture.loadTexture(imageFileName);

    if (status) {
        textures[textName] = texture;
    }

    return status;
}

void TextureManager::setTextureUnit(unsigned int samplerIndex) {
    if (samplerHandler != 0) {
        glUniform1i(samplerHandler, 0);
    }
}

void TextureManager::enableTexture(std::string textName, GLenum textureUnit) {
    if (textures.find(textName) != textures.end()) {
        textures[textName].enable(textureUnit);
    }
}

void TextureManager::disableTexture(std::string textName) {
    if (textures.find(textName) != textures.end()) {
        textures[textName].disable();
    }
}

