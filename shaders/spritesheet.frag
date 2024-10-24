#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;
uniform vec2 uv_offset;
uniform vec2 uv_step;

void main()
{
    vec2 uv = uv_step * (TexCoords + uv_offset);
    color = vec4(spriteColor, 1.0) * texture(image, uv);
}