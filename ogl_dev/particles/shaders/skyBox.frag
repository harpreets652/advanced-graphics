#version 330

in vec3 texCoord;

out vec4 frag_color;

uniform samplerCube gSkyBoxTexture;

void main() {
    frag_color = texture(gSkyBoxTexture, texCoord);
}