//
// Created by Harpreet Singh on 9/24/17.
//

#ifndef LIGHTING_LIGHTINGMODEL_H
#define LIGHTING_LIGHTINGMODEL_H

#include "graphics_headers.h"
#include "shader.h"
#include <limits.h>
#include <string.h>

/*
 * using some code from http://ogldev.atspace.co.uk
 */
struct BaseLight {
    glm::vec3 color;
    float ambientIntensity;
    float diffuseIntensity;

    BaseLight() {
        color = glm::vec3(0.0f, 0.0f, 0.0f);
        ambientIntensity = 0.0f;
        diffuseIntensity = 0.0f;
    }
};

struct DirectionalLight : public BaseLight {
    glm::vec3 direction;

    DirectionalLight() {
        direction = glm::vec3(0.0f, 0.0f, 0.0f);
    }
};

struct PointLight : public BaseLight {
    glm::vec3 position;

    struct {
        float constant;
        float linear;
        float exp;
    } attenuation;

    PointLight() {
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        attenuation.constant = 1.0f;
        attenuation.linear = 0.0f;
        attenuation.exp = 0.0f;
    }
};

struct SpotLight : public PointLight {
    glm::vec3 direction;
    float cutoff;

    SpotLight() {
        direction = glm::vec3(0.0f, 0.0f, 0.0f);
        cutoff = 0.0f;
    }
};

class LightingModel {
    static const unsigned int MAX_POINT_LIGHTS = 2;
    static const unsigned int MAX_SPOT_LIGHTS = 2;

public:
    LightingModel();

    //find shader handlers and store them
    bool initialize(Shader &shaderManager);

    //set all lighting variables in shaders
    void renderLighting();

private:
    void setDirectionalLight(const DirectionalLight &dLight);

    void setPointLights(const int numLights, const PointLight *pLights);

    void setSpotLights(const int numLights, const SpotLight *pLights);

    GLint numPointLightsLocation;
    GLint numSpotLightsLocation;

    struct {
        GLint color;
        GLint ambientIntensity;
        GLint diffuseIntensity;
        GLint direction;
    } dirLightLocation;

    struct {
        GLint color;
        GLint ambientIntensity;
        GLint diffuseIntensity;
        GLint position;
        struct {
            GLint constant;
            GLint linear;
            GLint exp;
        } atten;
    } pointLightsLocation[MAX_POINT_LIGHTS];

    struct {
        GLint Color;
        GLint AmbientIntensity;
        GLint DiffuseIntensity;
        GLint Position;
        GLint Direction;
        GLint Cutoff;
        struct {
            GLint Constant;
            GLint Linear;
            GLint Exp;
        } Atten;
    } spotLightsLocation[MAX_SPOT_LIGHTS];
};


#endif //LIGHTING_LIGHTINGMODEL_H
