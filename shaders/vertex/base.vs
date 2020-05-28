#version 330 core
layout (location = 0) in vec3 Vertex;
layout (location = 1) in vec3 ColorVec;
layout (location = 2) in vec2 TextureVec;
  
out vec3 Fragment;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(Vertex, 1.0);
    Fragment = ColorVec;
    TexCoord  = TextureVec;
}
