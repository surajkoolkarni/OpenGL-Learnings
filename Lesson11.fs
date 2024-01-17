#version 330 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform Material material;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    // ambient light
    vec3 ambient = material.ambient * lightColor;

    // diffuse light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.);
    vec3 diffuse = diff * material.diffuse * lightColor;

    // specular light
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectionDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), 32); // power 32 shininess value
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, 1.0f);
}
