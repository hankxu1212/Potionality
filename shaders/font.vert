#version 330

uniform mat4 CLIP_FROM_LOCAL;
in vec4 Position;
in vec2 TexCoord;

out vec2 texCoord;

void main() {
    gl_Position = CLIP_FROM_LOCAL * Position;
    texCoord = TexCoord;
}