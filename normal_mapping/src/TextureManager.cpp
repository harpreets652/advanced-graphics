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
    colorSamplerHandler = 0;
}

bool TextureManager::initHandlers(Shader &shaderManager) {
    colorSamplerHandler = shaderManager.getUniformLocation("gSampler");
    normalSamplerHandler = shaderManager.getUniformLocation("gNormalMapSampler");

    return colorSamplerHandler != INVALID_UNIFORM_LOCATION &&
           normalSamplerHandler != INVALID_UNIFORM_LOCATION;
}

bool TextureManager::addTexture(const std::string textName, std::string imageFileName) {
    auto *texture = new Texture;
    bool status = texture->loadTexture(imageFileName);

    if (status) {
        textures.insert({textName, texture});
    }

    return status;
}

void TextureManager::setColorTextureUnit(unsigned int samplerIndex) {
    if (colorSamplerHandler != 0) {
        glUniform1i(colorSamplerHandler, samplerIndex);
    }
}

void TextureManager::setNormalMapTextureUnit(unsigned int samplerIndex) {
    if (normalSamplerHandler != 0) {
        glUniform1i(normalSamplerHandler, samplerIndex);
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

