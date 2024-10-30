#version 330 core

in vec2 TexCoords;
out vec4 outColor;

uniform sampler2D image;
uniform vec3 spriteColor;
uniform vec2 uv_offset;
uniform vec2 uv_step;

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
    vec2 uv = uv_step * (TexCoords + uv_offset);
    vec4 texColor = texture(image, uv);
    if (texColor.a == 0)
        discard;

    float D = distance(gl_FragCoord.xy, lightPosition);
    float falloff = CalculateAttenuation(D, 1500, 0.001);

    vec3 finalColor = 1 * falloff * spriteColor * texColor.rgb;
    
    outColor = vec4(finalColor, texColor.a);
}