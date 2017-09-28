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

bool TextureManager::initHandlers(Shader shaderProgram) {
    samplerHandler = shaderProgram.getUniformLocation("gSampler");

    if (samplerHandler == INVALID_UNIFORM_LOCATION) {
        return false;
    }

    return true;
}

bool TextureManager::loadTexture(const std::string textName, std::string imageFileName) {
    Texture texture;
    bool status = texture.loadTexture(imageFileName);

    if (status) {
        textures[textName] = texture;
    }

    return status;
}

void TextureManager::setSamplerIndex(int samplerIndex) {
    if (samplerHandler != 0) {
        glUniform1i(samplerHandler, samplerIndex);
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

