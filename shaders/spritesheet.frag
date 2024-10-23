#version 330 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 spriteColor;
uniform float uv_x;
uniform float uv_y;
uniform float nx_frames;
uniform float ny_frames;

void main()
{
    float x = 1.0 / nx_frames;
    float y = 1.0 / ny_frames;

    vec2 uv = vec2(TexCoords.x * x, TexCoords.y * y) + vec2(x * uv_x, y * uv_y);

    color = vec4(spriteColor, 1.0) * texture(image, uv);
}