#version 330

uniform sampler2D TEX;
in vec2 texCoord;
out vec4 fragColor;

void main() {
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(TEX, texCoord).r * 5);
    fragColor = sampled;
}