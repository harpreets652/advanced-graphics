#version 330

in vec2 TextureCoord0;

uniform sampler2D gShadowMap;

out vec4 frag_color;

void main() {
    float depthVal = texture(gShadowMap, TextureCoord0.xy).x;
    depthVal = 1.0 - (1.0 - depthVal) * 500.0;
    frag_color = vec4(depthVal);
}