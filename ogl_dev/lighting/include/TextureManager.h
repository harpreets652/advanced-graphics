//
// Created by Harpreet Singh on 9/23/17.
//

#ifndef LIGHTING_TEXTUREMANAGER_H
#define LIGHTING_TEXTUREMANAGER_H

#include "Texture.h"
#include <unordered_map>

class TextureManager {
public:
    static TextureManager *getInstance();

    bool loadTexture(std::string &textName, std::string fileName);

    void enableTexture(std::string textName);

    void disableTexture(std::string textName);

private:
    TextureManager() {};

    static TextureManager *instance;
    std::unordered_map<std::string, Texture> textures;
};

#endif //LIGHTING_TEXTUREMANAGER_H
