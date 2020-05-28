#version 330 core
layout(location = 0) out vec4 Color;

in vec3 Fragment;

void main()
{
    Color = vec4(Fragment, 1.0);
}
