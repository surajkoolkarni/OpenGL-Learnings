#version 330 core

layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;
out vec3 LightDir;
out vec3 Specular;
out vec3 LightColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;


void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * vec4(FragPos, 1.0f);
    vec3 norm = normalize(Normal);

    Normal = mat3(transpose(inverse(model))) * aNormal;

    // specular light
    LightDir = normalize(lightPos - FragPos);
    LightColor = lightColor;

    float specularStrength = 0.9;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectionDir = reflect(-LightDir, norm);

    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), 256); // power 32 shininess value
    Specular = specularStrength * spec * lightColor;
}
