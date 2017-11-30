//
// Created by Harpreet Singh on 10/26/17.
//

#include "Billboard.h"

static const int NUM_OF_ROWS = 4;
static const int NUM_OF_COLS = 11;

Billboard::Billboard() {
    billboardTexture = new Texture();
    vertexBuffer = 0;
}

Billboard::~Billboard() {
}

bool Billboard::initialize(std::string pTextureName) {
    //load billboard texture
    if (!billboardTexture->loadTexture(pTextureName)) {
        return false;
    }

    //initialize shader
    billboardShader = new Shader();
    if (!billboardShader->initialize()) {
        std::cout << "Billboard shader program failed to initialize" << std::endl;
        return false;
    }

    //add vertex, geometry, and fragment shader
    if (!billboardShader->addShaderFromFile("../shaders/billboard.vert", GL_VERTEX_SHADER)) {
        std::cout << "Vertex shader failed to initialize" << std::endl;
        return false;
    }

    if (!billboardShader->addShaderFromFile("../shaders/billboard.geom", GL_GEOMETRY_SHADER)) {
        std::cout << "Fragment Shader failed to Initialize" << std::endl;
        return false;
    }

    if (!billboardShader->addShaderFromFile("../shaders/billboard.frag", GL_FRAGMENT_SHADER)) {
        std::cout << "Fragment Shader failed to Initialize" << std::endl;
        return false;
    }

    if (!billboardShader->finalize()) {
        std::cout << "Failed to finalize billboard shader program" << std::endl;
        return false;
    }

    projectViewMatrixHandler = billboardShader->getUniformLocation("projectViewMatrix");
    cameraPositionHandler = billboardShader->getUniformLocation("cameraPosition");
    billboardSamplerHandler = billboardShader->getUniformLocation("billboardSampler");
    billboardSizeHandler = billboardShader->getUniformLocation("gBillboardSize");

    //define positions of billboards
    glm::vec3 billboardPositions[NUM_OF_ROWS * NUM_OF_COLS];

    int xOffset = -19;
    int zOffset = -9;
    float xDiff = 5.0f;
    for (unsigned int j = 0 ; j < NUM_OF_ROWS; j++) {
        for (unsigned int i = 0 ; i < NUM_OF_COLS; i++) {
            glm::vec3 position;
            if (j % 2 == 0) {
                position = glm::vec3(((float)i * 3 + xOffset) + xDiff , 0.0f, (float)j * 2.0 + zOffset);
            } else {
                position = glm::vec3((float)i * 3 + xOffset , 0.0f, (float)j * 2.0 + zOffset);
            }

            billboardPositions[j * NUM_OF_COLS + i] = position;
        }
    }

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(billboardPositions), &billboardPositions[0], GL_STATIC_DRAW);

    return true;
}

void Billboard::render(glm::mat4 pProjectionViewMat, glm::vec3 pCameraPosition) {
    billboardShader->enable();

    glUniformMatrix4fv(projectViewMatrixHandler, 1, GL_FALSE, glm::value_ptr(pProjectionViewMat));
    glUniform3f(cameraPositionHandler, pCameraPosition.x, pCameraPosition.y, pCameraPosition.z);

    glUniform1i(billboardSamplerHandler, 1);
    billboardTexture->enable(GL_TEXTURE1);
    glUniform1f(billboardSizeHandler, 6.0f);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_POINTS, 0, NUM_OF_ROWS * NUM_OF_COLS);

    glDisableVertexAttribArray(0);
}