#version 330 core

struct Material
{
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material;
uniform SpotLight light;

uniform vec3 viewPos;

void main()
{
    // ambient light
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    // diffuse light
    vec3 norm = normalize(Normal);

    float dist = length(light.position - FragPos);
    float attenuation = 1. / (light.constant + (light.linear * dist) + (light.quadratic * dist * dist));

    // vec3 lightDir = normalize(-light.direction);
    vec3 lightDir = normalize(light.position - FragPos);

    float theta = dot(lightDir, normalize(-light.direction));

    if (theta > light.cutoff)
    {
        float diff = max(dot(norm, lightDir), 0.);
        vec3 diffuse = diff * vec3(texture(material.diffuse, TexCoords)) * light.diffuse;

        // specular light
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectionDir = reflect(-lightDir, norm);

        float spec = pow(max(dot(viewDir, reflectionDir), 0.0), material.shininess); // power 32 shininess value
        vec3 specular = material.specular * spec * light.specular;

        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        vec3 result = (ambient + diffuse + specular);

        FragColor = vec4(result, 1.0f);
    }
    else
        FragColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0f);
}
