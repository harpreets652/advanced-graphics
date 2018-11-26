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

using namespace std;


class Graphics {
public:
    Graphics();

    ~Graphics();

    bool Initialize(int width, int height);

    void cameraInput(SDL_Keycode input);

    void Update(unsigned int dt);

    void Render();

private:
    void renderPass();

    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;
    LightingModel *lightingModel;
    Billboard *billboardRenderer;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    Object *m_board;
    Object *m_chessPiece;
};

#endif /* GRAPHICS_H */
