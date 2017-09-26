//
// Created by Harpreet Singh on 9/23/17.
//

#ifndef LIGHTING_TEXTUREMANAGER_H
#define LIGHTING_TEXTUREMANAGER_H

#include <unordered_map>
#include "Texture.h"

class TextureManager {
public:
    static TextureManager *getInstance();

    bool loadTexture(const std::string textName, std::string fileName);

    void enableTexture(std::string textName);

    void disableTexture(std::string textName);

private:
    TextureManager() {};

    static TextureManager *instance;
    std::unordered_map<std::string, Texture> textures;
};

#endif //LIGHTING_TEXTUREMANAGER_H
