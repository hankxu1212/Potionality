#version 330 core

in vec2 TexCoords;
out vec4 outColor;

uniform sampler2D image;
uniform vec2 lightPosition;
uniform vec2 textureDims;

vec2 uv_cstantos( vec2 uv, vec2 res ) {
    vec2 pixels = uv * res;

    // Updated to the final article
    vec2 alpha = 0.7 * fwidth(pixels);
    vec2 pixels_fract = fract(pixels);
    vec2 pixels_diff = clamp( .5 / alpha * pixels_fract, 0, .5 ) +
                       clamp( .5 / alpha * (pixels_fract - 1) + .5, 0, .5 );
    pixels = floor(pixels) + pixels_diff;
    return pixels / res;
}

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
    vec2 uv = uv_cstantos(TexCoords, textureDims);
    vec4 texColor = texture(image, uv);
    if (texColor.a == 0)
        discard;

    float D = distance(gl_FragCoord.xy, lightPosition);
    float falloff = CalculateAttenuation(D, 1500, 0.0001);

    vec3 tint = 1 * falloff * texColor.rgb;
    outColor = vec4(tint, texColor.a);
}