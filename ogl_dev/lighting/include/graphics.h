#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
#include <SDL2/SDL_keycode.h>

#include "graphics_headers.h"
#include "camera.h"
#include "Direction.h"
#include "shader.h"
#include "object.h"
//#include "TextureManager.h" Note~ add this again
#include "Texture.h"
#include "LightingModel.h"

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
    std::string ErrorString(GLenum error);

    Camera *m_camera;
    Shader *m_shader;
    LightingModel *lightingModel;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    Object *m_board;
};

#endif /* GRAPHICS_H */
