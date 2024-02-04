#version 330 core

struct Material
{
    sampler2D diffuse;
    vec3 specular;
    float shininess;
};

struct DirectionalLight
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
    float outerCutoff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material;

uniform SpotLight spotLight;
uniform PointLight pointLights[4];
uniform DirectionalLight directionalLight;

uniform vec3 viewPos;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir); 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // diffuse light
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(directionalLight, norm, viewDir);

    for (int i = 0; i < 4; ++i)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

    result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0f);
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.);
    vec3 diffuse = diff * vec3(texture(material.diffuse, TexCoords)) * light.diffuse;

    vec3 reflectionDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), material.shininess); // power 32 shininess value
    vec3 specular = material.specular * spec * light.specular;

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) 
{
    float dist = length(light.position - fragPos);
    float attenuation = 1. / (light.constant + (light.linear * dist) + (light.quadratic * dist * dist));

    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.);
    vec3 diffuse = diff * vec3(texture(material.diffuse, TexCoords)) * light.diffuse;

    vec3 reflectionDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), material.shininess); // power 32 shininess value
    vec3 specular = light.specular * spec * material.specular;

    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    // ambient light
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    vec3 lightDir = normalize(light.position - fragPos);

    // smoothening intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0., 1.);

    float diff = max(dot(normal, lightDir), 0.);
    vec3 diffuse = diff * vec3(texture(material.diffuse, TexCoords)) * light.diffuse;

    vec3 reflectionDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), material.shininess); // power 32 shininess value
    vec3 specular = material.specular * spec * light.specular;

    diffuse *= intensity;
    specular *= intensity;

    return ambient + diffuse + specular;
}
