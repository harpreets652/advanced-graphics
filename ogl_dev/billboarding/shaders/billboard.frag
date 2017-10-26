#version 330

uniform sampler2D billboardSampler;

in vec2 TexCoord;
out vec4 frag_color;

void main() {
    frag_color = texture(billboardSampler, TexCoord);

    if (frag_color.r == 0 && frag_color.g == 0 && frag_color.b == 0) {
        discard;
    }
}