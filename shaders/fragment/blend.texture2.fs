#version 330 core
layout(location = 0) out vec4 Color;

in vec3 Fragment;
in vec2 TexCoord;

uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
uniform float uBlend = 0.5;

void main()
{
    Color = mix
    (
        texture(uTexture1, TexCoord),
        texture(uTexture2, TexCoord),
        uBlend
    );
}
