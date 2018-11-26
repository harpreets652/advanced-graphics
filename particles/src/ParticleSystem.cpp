//
// Created by Harpreet Singh on 11/10/17.
//

#include "ParticleSystem.h"

#define MAX_PARTICLES 2000
#define PARTICLE_LIFETIME 10.0f

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0f

struct Particle {
    float type;
    glm::vec3 position;
    glm::vec3 velocity;
    float lifetimeMillis;
};

ParticleSystem::ParticleSystem() {
    currentVB = 0;
    currentTransFeedBuff = 1;
    isFirstRender = true;
    m_time = 0;
    particleTexture = NULL;
    queryResult = 0;
}

ParticleSystem::~ParticleSystem() {
    if (transformFeedbackBuffer[0] != 0) {
        glDeleteTransformFeedbacks(2, transformFeedbackBuffer);
    }

    if (particleBuffer[0] != 0) {
        glDeleteBuffers(2, particleBuffer);
    }
}

bool ParticleSystem::initialize(std::string pTextureName) {
    Particle particles[MAX_PARTICLES];

    particles[0].type = PARTICLE_TYPE_LAUNCHER;
    particles[0].position = glm::vec3(0.0f, 4.5f, 6.0f);
    particles[0].velocity = glm::vec3(0.0f, 10.0f, 0.0f);
    particles[0].lifetimeMillis = 0.0f;

    glGenTransformFeedbacks(2, transformFeedbackBuffer);
    glGenBuffers(2, particleBuffer);

    for (unsigned int i = 0; i < 2; i++) {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transformFeedbackBuffer[i]);
        glBindBuffer(GL_ARRAY_BUFFER, particleBuffer[i]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, particleBuffer[i]);
    }

    // load shader objects: particle billboards AND particle update shader
    particleBillboardShader = new Shader();
    if (!initializeShader(particleBillboardShader,
                          "../shaders/particleBillboard.vert",
                          "../shaders/particleBillboard.geom",
                          "../shaders/particleBillboard.frag")) {
        return false;
    }

    if (!particleBillboardShader->finalize()) {
        std::cout << "Failed to finalize shader program" << std::endl;
        return false;
    }

    particleUpdateShader = new Shader();
    if (!initializeShader(particleUpdateShader,
                          "../shaders/particleSystem.vert",
                          "../shaders/particleSystem.geom",
                          "../shaders/particleSystem.frag")) {
        return false;
    }

    //set varying for transform feedback before finalizing shader
    const GLchar *Varyings[4];
    Varyings[0] = "Type1";
    Varyings[1] = "Position1";
    Varyings[2] = "Velocity1";
    Varyings[3] = "Age1";

    glTransformFeedbackVaryings(particleUpdateShader->getShaderProgram(), 4, Varyings, GL_INTERLEAVED_ATTRIBS);

    if (!particleUpdateShader->finalize()) {
        std::cout << "Failed to finalize shader program" << std::endl;
        return false;
    }

    if (!getParticleUpdateHandlers()) {
        std::cout << "Failed to get all handlers from particle update program" << std::endl;
        return false;
    }

    if (!particleBillboardHandlers()) {
        std::cout << "Failed to get all handlers from particle billboard update program" << std::endl;
        return false;
    }

    randomTexture = new RandomTexture();
    if (!randomTexture->initialize(1000)) {
        return false;
    }

    setInitialParticleProperties();
    setInitialParticleBillboardProperties();

    particleTexture = new Texture();
    if (!particleTexture->loadTexture(pTextureName)) {
        std::cout << "Failed to load texture " << pTextureName << std::endl;
        return false;
    }

    glGenQueries(1, &numPrimitivesGenerated);

    auto errorCode = glGetError();
    if (errorCode != GL_NO_ERROR) {
        std::cout << "GL ERRORS DETECTED: " << errorCode << std::endl;
        return false;
    }

    return true;
}

bool ParticleSystem::particleBillboardHandlers() {
    projectViewMatrixHandler = particleBillboardShader->getUniformLocation("projectViewMatrix");
    cameraPositionHandler = particleBillboardShader->getUniformLocation("cameraPosition");
    particleSamplerHandler = particleBillboardShader->getUniformLocation("billboardParticleSampler");
    billboardSizeHandler = particleBillboardShader->getUniformLocation("gBillboardSize");

    return !(projectViewMatrixHandler == INVALID_UNIFORM_LOCATION ||
             cameraPositionHandler == INVALID_UNIFORM_LOCATION ||
             billboardSizeHandler == INVALID_UNIFORM_LOCATION ||
             particleSamplerHandler == INVALID_UNIFORM_LOCATION);
}

bool ParticleSystem::getParticleUpdateHandlers() {
    m_deltaTimeMillisHandler = particleUpdateShader->getUniformLocation("gDeltaTimeMillis");
    m_timeHandler = particleUpdateShader->getUniformLocation("gTime");
    m_randomTextureSamplerHandler = particleUpdateShader->getUniformLocation("gRandomTexture");
    m_launcherLifetimeHandler = particleUpdateShader->getUniformLocation("gLauncherLifetime");
    numToGenerateHandler = particleUpdateShader->getUniformLocation("numToGenerate");

    return !(m_deltaTimeMillisHandler == INVALID_UNIFORM_LOCATION ||
             m_timeHandler == INVALID_UNIFORM_LOCATION ||
             m_randomTextureSamplerHandler == INVALID_UNIFORM_LOCATION ||
             m_launcherLifetimeHandler == INVALID_UNIFORM_LOCATION);
}

void ParticleSystem::setInitialParticleProperties() {
    particleUpdateShader->enable();
    // this is in milliseconds
    glUniform1i(m_randomTextureSamplerHandler, 4);
    glUniform1f(m_launcherLifetimeHandler, 100.0f);
}

void ParticleSystem::setInitialParticleBillboardProperties() {
    particleBillboardShader->enable();
    glUniform1i(particleSamplerHandler, 5);
    glUniform1f(billboardSizeHandler, 0.05f);
}

bool ParticleSystem::initializeShader(Shader *pShader,
                                      const std::string &pVert,
                                      const std::string &pGeom,
                                      const std::string &pFrag) {
    if (!pShader->initialize()) {
        std::cout << "Particle System shader program failed to initialize" << std::endl;
        return false;
    }

    if (!pShader->addShaderFromFile(pVert, GL_VERTEX_SHADER)) {
        std::cout << "Vertex shader failed to initialize" << std::endl;
        return false;
    }

    if (!pShader->addShaderFromFile(pGeom, GL_GEOMETRY_SHADER)) {
        std::cout << "Fragment Shader failed to Initialize" << std::endl;
        return false;
    }

    if (!pShader->addShaderFromFile(pFrag, GL_FRAGMENT_SHADER)) {
        std::cout << "Fragment Shader failed to Initialize" << std::endl;
        return false;
    }

    return true;
}

void ParticleSystem::render(unsigned int dt, Camera *pCamera) {
    m_time += dt;

    updateParticles(dt);

    glm::mat4 projectionView = pCamera->GetProjection() * pCamera->GetView();
    renderParticles(projectionView, pCamera->getPosition());

    currentVB = currentTransFeedBuff;
    currentTransFeedBuff = (currentTransFeedBuff + 1) & 0x1;
}

void ParticleSystem::updateParticles(unsigned int dt) {
    particleUpdateShader->enable();
    glUniform1f(m_timeHandler, (float) m_time);
    glUniform1f(m_deltaTimeMillisHandler, (float) dt);
    glUniform1i(numToGenerateHandler, 20);

    if (queryResult > MAX_PARTICLES) {
        glUniform1i(numToGenerateHandler, 0);
    }

    randomTexture->enable(GL_TEXTURE4);

    glEnable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, particleBuffer[currentVB]);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transformFeedbackBuffer[currentTransFeedBuff]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *) offsetof(Particle, position));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *) offsetof(Particle, velocity));
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *) offsetof(Particle, lifetimeMillis));

    glBeginTransformFeedback(GL_POINTS);
    glBeginQueryIndexed(GL_PRIMITIVES_GENERATED, 0, numPrimitivesGenerated);

    if (isFirstRender) {
        glDrawArrays(GL_POINTS, 0, 1);
        isFirstRender = false;
    } else {
        glDrawTransformFeedback(GL_POINTS, transformFeedbackBuffer[currentVB]);
    }

    glEndQueryIndexed(GL_PRIMITIVES_GENERATED, 0);
    glEndTransformFeedback();

    glGetQueryObjectiv(numPrimitivesGenerated, GL_QUERY_RESULT, &queryResult);
//    std::cout << "number of primitives: " << queryResult << std::endl;

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
}

void ParticleSystem::renderParticles(glm::mat4 pProjViewMat, glm::vec3 pCameraPosition) {
    particleBillboardShader->enable();

    glUniformMatrix4fv(projectViewMatrixHandler, 1, GL_FALSE, glm::value_ptr(pProjViewMat));
    glUniform3f(cameraPositionHandler, pCameraPosition.x, pCameraPosition.y, pCameraPosition.z);
    glUniform1i(particleSamplerHandler, 5);
    particleTexture->enable(GL_TEXTURE5);

    glDisable(GL_RASTERIZER_DISCARD);

    glBindBuffer(GL_ARRAY_BUFFER, particleBuffer[currentTransFeedBuff]);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void *) offsetof(Particle, position));

    glDrawTransformFeedback(GL_POINTS, transformFeedbackBuffer[currentTransFeedBuff]);

    glDisableVertexAttribArray(0);
}
