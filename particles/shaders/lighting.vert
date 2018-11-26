#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_textCoord;
layout (location = 2) in vec3 v_normal;
layout (location = 3) in vec3 v_color;
layout (location = 4) in vec3 v_tangent;

smooth out vec3 f_color;
out vec3 Normal0;
out vec3 WorldPos0;
out vec2 TextureCoord0;
out vec4 LightSpacePos;
out vec3 Tangent0;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 lightViewMatrix;

void main() {
    mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;
    gl_Position = mvp * vec4(v_position, 1.0);

    mat4 lightMVP = projectionMatrix * lightViewMatrix * modelMatrix;
    LightSpacePos = lightMVP * vec4(v_position, 1.0);

    Normal0 = (modelMatrix * vec4(v_normal, 0.0)).xyz;
    WorldPos0 = (modelMatrix * vec4(v_position, 1.0)).xyz;
    Tangent0 = (modelMatrix * vec4(v_tangent, 0.0)).xyz;
    TextureCoord0 = v_textCoord;
    f_color = v_color;
}