#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_textCoord;

out vec2 TextureCoord0;

uniform mat4 mvpMatrix;

void main() {
    gl_Position = mvpMatrix * vec4(v_position, 1.0);
    TextureCoord0 = v_textCoord;
}