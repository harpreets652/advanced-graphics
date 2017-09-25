//
// Created by Harpreet Singh on 9/23/17.
//

#include "TextureManager.h"

TextureManager *TextureManager::instance = nullptr;

TextureManager *TextureManager::getInstance() {
    if (!instance) {
        instance = new TextureManager;
        ilInit(); //Note~ remove this if not needed
    }

    return instance;
}

bool TextureManager::loadTexture(std::string &textName, std::string imageFileName) {
    Texture texture;
    bool status = texture.loadTexture(imageFileName);

    if (status) {
        textures.insert(std::make_pair(textName, texture));
    }

    return status;
}

void TextureManager::enableTexture(std::string textName) {
    if (textures.find(textName) != textures.end()) {
        textures[textName].enable();
    }
}

void TextureManager::disableTexture(std::string textName) {
    if (textures.find(textName) != textures.end()) {
        textures[textName].disable();
    }
}

