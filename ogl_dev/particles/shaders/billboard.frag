#version 330

uniform sampler2D billboardSampler;

in vec2 TexCoord;
out vec4 frag_color;

void main() {
    frag_color = texture(billboardSampler, TexCoord);

    float threshold = 0.009;
    if (frag_color.r < threshold && frag_color.g < threshold && frag_color.b < threshold) {
        discard;
    }
}