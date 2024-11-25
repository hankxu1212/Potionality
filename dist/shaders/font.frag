#version 330

uniform sampler2D TEX;
in vec2 texCoord;
out vec4 fragColor;

void main() {
    float alpha = texture(TEX, texCoord).r * 5;
    if (alpha < 0.5)
        discard;
    vec4 sampled = vec4(1.0, 1.0, 1.0, alpha);
    fragColor = sampled;
}