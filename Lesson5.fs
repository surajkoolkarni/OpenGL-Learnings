#version 330 core

out vec4 FragColor;
uniform sampler2D ourTexture;

in vec3 ourColor;
in vec2 texCoord;
  
void main()
{
    FragColor = texture(ourTexture, texCoord) * vec4(ourColor, 1.0f);
}
