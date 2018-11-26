#version 120

uniform float gScale;
attribute vec3 Position;

void main() {
    gl_Position = vec4(gScale * Position.x, gScale * Position.y, Position.z, 1.0);
}

