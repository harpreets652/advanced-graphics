//
// Created by Harpreet Singh on 9/23/17.
//

#ifndef LIGHTING_TEXTUREMANAGER_H
#define LIGHTING_TEXTUREMANAGER_H

#include <map>
#include "Texture.h"
#include "shader.h"

class TextureManager {
public:
    static TextureManager *getInstance();

    bool initHandlers(Shader &shaderManager);

    bool addTexture(const std::string textName, std::string fileName);

    void setTextureUnit(unsigned int samplerIndex);

    void enableTexture(std::string textName, GLenum textureUnit);

    void disableTexture(std::string textName);

private:
    TextureManager();

    static TextureManager *instance;
    GLint samplerHandler;
    std::map<std::string, Texture *> textures;
};

#endif //LIGHTING_TEXTUREMANAGER_H
