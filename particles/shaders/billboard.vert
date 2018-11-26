#version 330

//passthrough vertex shader
layout (location = 0) in vec3 v_position;

void main() {
    gl_Position = vec4(v_position, 1.0);
}