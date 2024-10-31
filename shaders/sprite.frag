#version 330 core

in vec2 TexCoords;
out vec4 outColor;

uniform sampler2D image;
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

void main()
{
    vec2 uv = uv_cstantos(TexCoords, textureDims);
    vec4 texColor = texture(image, uv);
    if (texColor.a <= 0.3)
        discard;

    vec3 tint = texColor.rgb;
    outColor = vec4(tint, texColor.a);
}