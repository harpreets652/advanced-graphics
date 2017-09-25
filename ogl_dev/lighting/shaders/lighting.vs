#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_color;
layout (location = 2) in vec3 v_normal;

smooth out vec3 f_color;
out vec3 Normal0;
out vec3 WorldPos0;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main() {
    mat4 gWVP = projectionMatrix * viewMatrix * modelMatrix;
    gl_Position = gWVP * vec4(v_position, 1.0);

    Normal0 = (modelMatrix * vec4(v_normal, 0.0)).xyz;
    WorldPos0 = (modelMatrix * vec4(v_position, 1.0)).xyz;
    f_color = v_color;
}