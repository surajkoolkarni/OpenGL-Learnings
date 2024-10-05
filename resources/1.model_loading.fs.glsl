#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse[8];
uniform sampler2D texture_specular[8];
uniform sampler2D texture_normal[8];
uniform sampler2D texture_height[8];

void main()
{    
    FragColor = texture(texture_diffuse[0], TexCoords);
}
