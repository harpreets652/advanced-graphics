#version 120

attribute vec3 Position;

void main() {
    gl_Position = vec4(0.5 * Position.x, 0.5 * Position.y, Position.z, 1.0);
}

