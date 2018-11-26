//
// Created by Harpreet Singh on 9/24/17.
//
#include "LightingModel.h"

LightingModel::LightingModel() {
}

bool LightingModel::initialize(Shader &shaderManager) {
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
    DirectionalLight directionalLight;
    directionalLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    directionalLight.ambientIntensity = 0.2f;
    directionalLight.diffuseIntensity = 0.5f;
    directionalLight.direction = glm::vec3(0.0, 1.0, 0.0);
//    setDirectionalLight(directionalLight);

    PointLight pl[2];
    pl[0].diffuseIntensity = 0.25f;
    pl[0].color = glm::vec3(1.0f, 0.0f, 0.0f);
    pl[0].ambientIntensity = 1.0;
    pl[0].position = glm::vec3(-5.0f, 1.0f, 0);
    pl[0].attenuation.linear = 0.5f;

    pl[1].diffuseIntensity = 0.25f;
    pl[1].color = glm::vec3(0.0f, 0.0f, 1.0f);
    pl[1].ambientIntensity = 2.0;
    pl[1].position = glm::vec3(5.0, 1.0f, 0);
    pl[1].attenuation.linear = 0.0;
    pl[1].attenuation.exp = 0.5;
//    setPointLights(2, pl);

    SpotLight sl[2];
    sl[0].diffuseIntensity = 0.25f;
    sl[0].color = glm::vec3(1.0f, 1.0f, 1.0f);
    sl[0].ambientIntensity = 2.0;
    sl[0].position = glm::vec3(-6.0f, 5.0f, 0.0f);
    sl[0].attenuation.linear = 0.1f;
    sl[0].attenuation.exp = 0.0f;
    sl[0].direction = glm::vec3(2.0f, -1.0f, 0.0f);
    sl[0].cutoff = 40.0f;

    sl[1].diffuseIntensity = 0.25f;
    sl[1].color = glm::vec3(1.0f, 1.0f, 1.0f);
    sl[1].ambientIntensity = 2.0;
    sl[1].position = glm::vec3(2.0, 5.0, 0.0);
    sl[1].attenuation.linear = 0.1f;
    sl[1].direction = glm::vec3(0.0, -1.0, 0.0);
    sl[1].cutoff = 20.0f;

    setSpotLights(1, sl);
}

/*
 * directional: relative to normals of surface
 * if using board, with normals facing up on top of surface and down under the surface,
 * set light direction to <0,1,0> and can see that the surface under is dark, only illuminated by ambient light.
 * Note: disable other lighting models for clearer effect
 * Sample configuration:
    directionalLight.color = glm::vec3(1.0f, 1.0f, 1.0f);
    directionalLight.ambientIntensity = 0.2f;
    directionalLight.diffuseIntensity = 0.5f;
    directionalLight.direction = glm::vec3(0.0, 1.0, 0.0);
 */
void LightingModel::setDirectionalLight(const DirectionalLight &dLight) {
    glm::vec3 normalizedDirection = glm::normalize(dLight.direction);

    glUniform3f(dirLightLocation.color, dLight.color.x, dLight.color.y, dLight.color.z);
    glUniform1f(dirLightLocation.ambientIntensity, dLight.ambientIntensity);
    glUniform3f(dirLightLocation.direction, normalizedDirection.x, normalizedDirection.y, normalizedDirection.z);
    glUniform1f(dirLightLocation.diffuseIntensity, dLight.diffuseIntensity);
}

//todo: assuming two point lights
/*
 * Similar to directional light; set the light position and then calculate light direction
 * at each vertex relative to the light position and the vertex position.
 * Leave attenuation.constant to 1 and change linear and exponential. Need to set ambient intensity
 * which will be used to calculate color; color will change based on the distance of the vertex to the light position.
 * Note: disable other lighting models for clearer effect
 * sample configuration (two point-lights):
    pl[0].diffuseIntensity = 0.25f;
    pl[0].color = glm::vec3(1.0f, 0.0f, 0.0f);
    pl[0].ambientIntensity = 1.0;
    pl[0].position = glm::vec3(-5.0f, 1.0f, 0);
    pl[0].attenuation.linear = 0.5f;

    pl[1].diffuseIntensity = 0.25f;
    pl[1].color = glm::vec3(0.0f, 0.0f, 1.0f);
    pl[1].ambientIntensity = 2.0;
    pl[1].position = glm::vec3(5.0f, 1.0f, 0);
    pl[1].attenuation.linear = 0.0f;
    pl[1].attenuation.exp = 0.5f;
 */
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

/*
 * Similar to point light, spot light has all the properties but with direction (similar to directional light).
 * Unlike directional light, the direction should be negative (towards the source).
 * To make spotlight sharper, reduce attenuation and increase ambient intensity.
 * Note: disable other lighting models for clearer effect
 * Sample configurations:
    sl[0].diffuseIntensity = 0.25f;
    sl[0].color = glm::vec3(1.0f, 0.0f, 0.0f);
    sl[0].ambientIntensity = 2.0;
    sl[0].position = glm::vec3(2.0, 5.0, 0.0);
    sl[0].attenuation.linear = 0.1f;
    sl[0].direction = glm::vec3(0.0, -1.0, 0.0);
    sl[0].cutoff = 20.0f;

    sl[1].diffuseIntensity = 0.25f;
    sl[1].color = glm::vec3(0.0f, 1.0f, 0.0f);
    sl[1].ambientIntensity = 2.0;
    sl[1].position = glm::vec3(-4.0f, 5.0f, 0.0f);
    sl[1].attenuation.linear = 0.1f;
    sl[1].attenuation.exp = 0.0f;
    sl[1].direction = glm::vec3(0.0f, -1.0f, 0.0f);
    sl[1].cutoff = 40.0f;
 */
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