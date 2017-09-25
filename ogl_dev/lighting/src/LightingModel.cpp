//
// Created by Harpreet Singh on 9/24/17.
//

#include <LightingModel.h>
#include "LightingModel.h"

LightingModel::LightingModel() {
}

bool LightingModel::initialize(Shader &shaderManager) {
    //Note~ need to set other model/view matrices
    dirLightLocation.color = shaderManager.getUniformLocation("gDirectionalLight.Base.Color");
    dirLightLocation.ambientIntensity = shaderManager.getUniformLocation("gDirectionalLight.Base.AmbientIntensity");
    dirLightLocation.diffuseIntensity = shaderManager.getUniformLocation("gDirectionalLight.Base.DiffuseIntensity");
    dirLightLocation.direction = shaderManager.getUniformLocation("gDirectionalLight.Direction");
    numPointLightsLocation = shaderManager.getUniformLocation("gNumPointLights");
    numSpotLightsLocation = shaderManager.getUniformLocation("gNumSpotLights");

    for (unsigned int i = 0; i < MAX_POINT_LIGHTS; i++) {
        char name[128];
        memset(name, 0, sizeof(name));
        snprintf(name, sizeof(name), "gPointLights[%d].Base.Color", i);
        pointLightsLocation[i].color = shaderManager.getUniformLocation(name);

        snprintf(name, sizeof(name), "gPointLights[%d].Base.AmbientIntensity", i);
        pointLightsLocation[i].ambientIntensity = shaderManager.getUniformLocation(name);

        snprintf(name, sizeof(name), "gPointLights[%d].Position", i);
        pointLightsLocation[i].position = shaderManager.getUniformLocation(name);

        snprintf(name, sizeof(name), "gPointLights[%d].Base.DiffuseIntensity", i);
        pointLightsLocation[i].diffuseIntensity = shaderManager.getUniformLocation(name);

        snprintf(name, sizeof(name), "gPointLights[%d].Atten.Constant", i);
        pointLightsLocation[i].atten.constant = shaderManager.getUniformLocation(name);

        snprintf(name, sizeof(name), "gPointLights[%d].Atten.Linear", i);
        pointLightsLocation[i].atten.linear = shaderManager.getUniformLocation(name);

        snprintf(name, sizeof(name), "gPointLights[%d].Atten.Exp", i);
        pointLightsLocation[i].atten.exp = shaderManager.getUniformLocation(name);

        if (pointLightsLocation[i].color == INVALID_UNIFORM_LOCATION ||
            pointLightsLocation[i].ambientIntensity == INVALID_UNIFORM_LOCATION ||
            pointLightsLocation[i].position == INVALID_UNIFORM_LOCATION ||
            pointLightsLocation[i].diffuseIntensity == INVALID_UNIFORM_LOCATION ||
            pointLightsLocation[i].atten.constant == INVALID_UNIFORM_LOCATION ||
            pointLightsLocation[i].atten.linear == INVALID_UNIFORM_LOCATION ||
            pointLightsLocation[i].atten.exp == INVALID_UNIFORM_LOCATION) {
            return false;
        }
    }

    for (unsigned int i = 0; i < MAX_SPOT_LIGHTS; i++) {
        char Name[128];
        memset(Name, 0, sizeof(Name));
        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.Color", i);
        spotLightsLocation[i].Color = shaderManager.getUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.AmbientIntensity", i);
        spotLightsLocation[i].AmbientIntensity = shaderManager.getUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Position", i);
        spotLightsLocation[i].Position = shaderManager.getUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Direction", i);
        spotLightsLocation[i].Direction = shaderManager.getUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Cutoff", i);
        spotLightsLocation[i].Cutoff = shaderManager.getUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Base.DiffuseIntensity", i);
        spotLightsLocation[i].DiffuseIntensity = shaderManager.getUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Constant", i);
        spotLightsLocation[i].Atten.Constant = shaderManager.getUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Linear", i);
        spotLightsLocation[i].Atten.Linear = shaderManager.getUniformLocation(Name);

        snprintf(Name, sizeof(Name), "gSpotLights[%d].Base.Atten.Exp", i);
        spotLightsLocation[i].Atten.Exp = shaderManager.getUniformLocation(Name);

        if (spotLightsLocation[i].Color == INVALID_UNIFORM_LOCATION ||
            spotLightsLocation[i].AmbientIntensity == INVALID_UNIFORM_LOCATION ||
            spotLightsLocation[i].Position == INVALID_UNIFORM_LOCATION ||
            spotLightsLocation[i].Direction == INVALID_UNIFORM_LOCATION ||
            spotLightsLocation[i].Cutoff == INVALID_UNIFORM_LOCATION ||
            spotLightsLocation[i].DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
            spotLightsLocation[i].Atten.Constant == INVALID_UNIFORM_LOCATION ||
            spotLightsLocation[i].Atten.Linear == INVALID_UNIFORM_LOCATION ||
            spotLightsLocation[i].Atten.Exp == INVALID_UNIFORM_LOCATION) {
            return false;
        }
    }

    return !(dirLightLocation.ambientIntensity == INVALID_UNIFORM_LOCATION ||
             dirLightLocation.color == INVALID_UNIFORM_LOCATION ||
             dirLightLocation.diffuseIntensity == INVALID_UNIFORM_LOCATION ||
             dirLightLocation.direction == INVALID_UNIFORM_LOCATION ||
             numPointLightsLocation == INVALID_UNIFORM_LOCATION ||
             numSpotLightsLocation == INVALID_UNIFORM_LOCATION
    );
}

void LightingModel::renderLighting() {
    const float fieldDepth = 20.0f;
    const float m_scale = 0.0057f;

    DirectionalLight directionalLight;
    directionalLight.color = glm::vec3(0.0f, 0.0f, 0.0f);
    directionalLight.ambientIntensity = 0.5f;
    directionalLight.diffuseIntensity = 0.1f;
    directionalLight.direction = glm::vec3(1.0, 7.0, 0.0);
    setDirectionalLight(directionalLight);

    PointLight pl[2];
    pl[0].diffuseIntensity = 0.25f;
    pl[0].color = glm::vec3(1.0f, 0.5f, 0.0f);
    pl[0].position = glm::vec3(3.0f, 1.0f, fieldDepth * (cosf(m_scale) + 1.0f) / 2.0f);
    pl[0].attenuation.linear = 0.1f;

    pl[1].diffuseIntensity = 0.5f;
    pl[1].color = glm::vec3(0.0f, 0.5f, 1.0f);
    pl[1].position = glm::vec3(7.0f, 1.0f, fieldDepth * (cosf(m_scale) + 1.0f) / 2.0f);
    pl[1].attenuation.linear = 0.1f;
    setPointLights(0, pl);

    SpotLight sl[2];
    sl[0].diffuseIntensity = 0.9f;
    sl[0].color = glm::vec3(1.0f, 1.0f, 1.0f);
    sl[0].position = glm::vec3(0.0, 7.0, -18.0);
    sl[0].direction = glm::vec3(0.0, 3.0, -18.0);
    sl[0].attenuation.linear = 0.1f;
    sl[0].cutoff = 10.0f;

    sl[1].diffuseIntensity = 0.9f;
    sl[1].color = glm::vec3(1.0f, 1.0f, 1.0f);
    sl[1].position = glm::vec3(5.0f, 3.0f, 10.0f);
    sl[1].direction = glm::vec3(0.0f, -1.0f, 0.0f);
    sl[1].attenuation.linear = 0.1f;
    sl[1].cutoff = 20.0f;
    setSpotLights(0, sl);
}

void LightingModel::setDirectionalLight(const DirectionalLight &dLight) {
    glm::vec3 normalizedDirection = glm::normalize(dLight.direction);

    glUniform3f(dirLightLocation.color, dLight.color.x, dLight.color.y, dLight.color.z);
    glUniform1f(dirLightLocation.ambientIntensity, dLight.ambientIntensity);
    glUniform3f(dirLightLocation.direction, normalizedDirection.x, normalizedDirection.y, normalizedDirection.z);
    glUniform1f(dirLightLocation.diffuseIntensity, dLight.diffuseIntensity);
}

//todo: assuming two point lights
void LightingModel::setPointLights(const int numLights, const PointLight *pLights) {
    glUniform1i(numPointLightsLocation, numLights);

    for (unsigned int i = 0; i < numLights; i++) {
        glUniform3f(pointLightsLocation[i].color, pLights[i].color.x, pLights[i].color.y, pLights[i].color.z);
        glUniform1f(pointLightsLocation[i].ambientIntensity, pLights[i].ambientIntensity);
        glUniform1f(pointLightsLocation[i].diffuseIntensity, pLights[i].diffuseIntensity);
        glUniform3f(pointLightsLocation[i].position, pLights[i].position.x, pLights[i].position.y,
                    pLights[i].position.z);
        glUniform1f(pointLightsLocation[i].atten.constant, pLights[i].attenuation.constant);
        glUniform1f(pointLightsLocation[i].atten.linear, pLights[i].attenuation.linear);
        glUniform1f(pointLightsLocation[i].atten.exp, pLights[i].attenuation.exp);
    }
}

void LightingModel::setSpotLights(const int numLights, const SpotLight *pLights) {
    glUniform1i(numSpotLightsLocation, numLights);

    for (unsigned int i = 0; i < numLights; i++) {
        glUniform3f(spotLightsLocation[i].Color, pLights[i].color.x, pLights[i].color.y, pLights[i].color.z);
        glUniform1f(spotLightsLocation[i].AmbientIntensity, pLights[i].ambientIntensity);
        glUniform1f(spotLightsLocation[i].DiffuseIntensity, pLights[i].diffuseIntensity);
        glUniform3f(spotLightsLocation[i].Position, pLights[i].position.x, pLights[i].position.y,
                    pLights[i].position.z);

        glm::vec3 direction = glm::normalize(pLights[i].direction);
        glUniform3f(spotLightsLocation[i].Direction, direction.x, direction.y, direction.z);
        glUniform1f(spotLightsLocation[i].Cutoff, cosf(glm::radians(pLights[i].cutoff)));
        glUniform1f(spotLightsLocation[i].Atten.Constant, pLights[i].attenuation.constant);
        glUniform1f(spotLightsLocation[i].Atten.Linear, pLights[i].attenuation.linear);
        glUniform1f(spotLightsLocation[i].Atten.Exp, pLights[i].attenuation.exp);
    }
}