#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec2 uv_offset;
uniform vec2 uv_step;

void main()
{
    vec2 uv = uv_step * (TexCoords + uv_offset);
    color = texture(image, uv);
}