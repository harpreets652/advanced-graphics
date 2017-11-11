//
// Created by Harpreet Singh on 11/10/17.
//

#ifndef PARTICLES_PARTICLESYSTEM_H
#define PARTICLES_PARTICLESYSTEM_H

#include <algorithm>
#include "graphics_headers.h"
#include "shader.h"
#include "Texture.h"
#include "camera.h"
#include "RandomTexture.h"

class ParticleSystem {
public:
    ParticleSystem();

    ~ParticleSystem();

    bool initialize(std::string pTextureName, glm::vec3 pParticlePos);

    void render(unsigned int dt, Camera *pCamera);

private:
    void updateParticles(unsigned int dt);

    void renderParticles(glm::mat4 pProjViewMat, glm::vec3 pCameraPosition);

    bool initializeShader(Shader *pShader, const std::string &pVert,
                          const std::string &pGeom, const std::string &pFrag);

    bool getParticleUpdateHandlers();

    void setInitialParticleProperties();

    void setInitialParticleBillboardProperties();

    bool particleBillboardHandlers();

    bool isFirstRender;

    unsigned int currentVB;
    //transform feedback buffer
    unsigned int currentTransFeedBuff;
    GLuint particleBuffer[2];
    GLuint transformFeedbackBuffer[2];

    Shader *particleUpdateShader;
    Shader *particleBillboardShader;
    Texture *particleTexture;
    RandomTexture *randomTexture;

    GLint cameraPositionHandler;
    GLint projectViewMatrixHandler;
    GLint particleSamplerHandler;
    GLint billboardSizeHandler;
    int m_time;

    //update shader handler
    GLint m_deltaTimeMillisHandler;
    GLint m_randomTextureSamplerHandler;
    GLint m_timeHandler;
    GLint m_launcherLifetimeHandler;
    GLint m_shellLifetimeHandler;
    GLint m_secondaryShellLifetimeHandler;
};


#endif //PARTICLES_PARTICLESYSTEM_H
