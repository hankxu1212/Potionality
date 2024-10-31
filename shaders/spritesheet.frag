#version 330 core

in vec2 TexCoords;
out vec4 outColor;

uniform sampler2D image;
uniform vec2 uv_offset;
uniform vec2 uv_step;
uniform vec2 textureDims;

void main()
{
    vec2 uv = uv_step * (TexCoords + uv_offset);
    vec4 texColor = texture(image, uv);
    if (texColor.a <= 0.3)
        discard;

    vec3 finalColor = texColor.rgb;
    
    outColor = vec4(finalColor, texColor.a);
}