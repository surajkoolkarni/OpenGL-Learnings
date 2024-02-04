#version 330 core

struct Material
{
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

void main()
{
    // ambient light
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    // diffuse light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.);
    vec3 diffuse = diff * vec3(texture(material.diffuse, TexCoords)) * light.diffuse;

    // specular light
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectionDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), material.shininess); // power 32 shininess value
    vec3 specular = material.specular * spec * light.specular;

    vec3 result = (ambient + diffuse + specular);

    FragColor = vec4(result, 1.0f);
}
