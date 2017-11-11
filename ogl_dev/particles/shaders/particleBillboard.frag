#version 330

uniform sampler2D billboardParticleSampler;

in vec2 TexCoord;
out vec4 frag_color;

void main() {
    frag_color = texture(billboardParticleSampler, TexCoord);

    if (frag_color.r >= 0.9 && frag_color.g >= 0.9 && frag_color.b >= 0.9) {
        discard;
    }
}