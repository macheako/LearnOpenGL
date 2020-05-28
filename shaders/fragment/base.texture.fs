#version 330 core
layout(location = 0) out vec4 Color;

in vec3 Fragment;
in vec2 TexCoord;

uniform sampler2D uTexture;

void main()
{
    Color = texture(uTexture, TexCoord);
}
