#version 330

layout (location = 0) in vec3 v_position;

uniform mat4 mvpMatrix;

out vec3 texCoord;

void main() {
    vec4 MVP_Pos = mvpMatrix * vec4(v_position, 1.0);
    gl_Position = MVP_Pos.xyww;
    texCoord = v_position;
}
