#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec2 aTexCoord; // the color variable has attribute position 1

out vec2 texCoord; // output a texture coord to the fragment shader

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0f);
    texCoord = aTexCoord;
}
