#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <SDL2/SDL_keycode.h>

#include "graphics_headers.h"
#include "camera.h"
#include "Direction.h"
#include "shader.h"
#include "object.h"
#include "TextureManager.h"
#include "LightingModel.h"
#include "ShadowMap.h"
#include "Billboard.h"
#include "SkyBox.h"
#include "ParticleSystem.h"

using namespace std;


class Graphics {
public:
    Graphics() {}

    ~Graphics() {}

    bool Initialize(int width, int height);

    void cameraInput(SDL_Keycode input);

    void Update(unsigned int dt);

    void Render(unsigned int dt);

private:
    void renderPass(unsigned int dt);

    void skyBoxPass();

    void shadowPass();

    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;
    LightingModel *lightingModel;
    Billboard *billboardRenderer;
    SkyBox *skyBox;
    ParticleSystem *particleSystem;
    ShadowMap *shadowMap;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    Object *m_board;
    Object *m_chessPiece;
};

#endif /* GRAPHICS_H */
