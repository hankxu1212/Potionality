#version 330 core

in vec2 TexCoords;
out vec4 outColor;

uniform sampler2D image;
uniform vec3 spriteColor;
uniform vec2 lightPosition;

float CalculateAttenuation(float d, float radius, float falloff) {
    if (d <= radius) {
        return 1.0;
    } else {
        float falloffDistance = d - radius;
        return 1.0 / (1.0 + falloff * falloffDistance * falloffDistance);
    }
}

void main()
{
    vec4 texColor = texture(image, TexCoords);
    if (texColor.a == 0)
        discard;

    float D = distance(gl_FragCoord.xy, lightPosition);
    float falloff = CalculateAttenuation(D, 1500, 0.0001);

    vec3 tint = 1 * falloff * texColor.rgb;
    outColor = vec4(tint, texColor.a);
}