#version 330

in vec2 TexCoord;
out vec4 frag_color;

uniform sampler2D billboardSampler;

void main() {
    frag_color = texture(billboardSampler, TexCoord);

    float threshold = 0.7;
    if (frag_color.r > threshold && frag_color.g > threshold && frag_color.b > threshold) {
        discard;
    }
}