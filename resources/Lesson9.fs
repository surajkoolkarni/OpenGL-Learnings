#version 330 core

out vec4 FragColor;
uniform sampler2D ourTexture;

in vec2 texCoord;
  
void main()
{
    FragColor = texture(ourTexture, texCoord);
}
