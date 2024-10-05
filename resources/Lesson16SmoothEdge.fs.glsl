#version 460 core

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
in float TextureID;

uniform float shininess;

uniform SpotLight spotLight;
uniform PointLight pointLights[4];
uniform DirectionalLight directionalLight;

uniform sampler2D texture_diffuse[8];
uniform sampler2D texture_specular[8];
uniform sampler2D texture_normal[8];
uniform sampler2D texture_height[8];

uniform vec3 viewPos;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, sampler2D diffuseTex, sampler2D specularTex, float shininess);

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, sampler2D diffuseTex, sampler2D specularTex, float shininess); 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, sampler2D diffuseTex, sampler2D specularTex, float shininess);


void main()
{
    // diffuse light
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    uint idx = uint(TextureID);

    vec3 result = CalcDirLight(directionalLight, norm, viewDir, texture_diffuse[idx], texture_specular[idx], shininess);

    for (int i = 0; i < 4; ++i)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir, texture_diffuse[idx], texture_specular[idx], shininess);

    result += CalcSpotLight(spotLight, norm, FragPos, viewDir, texture_diffuse[idx], texture_specular[idx], shininess);

    FragColor = vec4(result, 1.0f);
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, sampler2D diffuseTex, sampler2D specularTex, float shininess)
{
    vec3 ambient = vec3(texture(diffuseTex, TexCoords)) * light.ambient;

    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.);
    vec3 diffuse = diff * vec3(texture(diffuseTex, TexCoords)) * light.diffuse;

    vec3 reflectionDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), shininess); // power 32 shininess value
    vec3 specular = spec * light.specular * vec3(texture(specularTex, TexCoords));

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, sampler2D diffuseTex, sampler2D specularTex, float shininess)
{
    float dist = length(light.position - fragPos);
    float attenuation = 1. / (light.constant + (light.linear * dist) + (light.quadratic * dist * dist));

    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.);
    vec3 diffuse = diff * vec3(texture(diffuseTex, TexCoords)) * light.diffuse;

    vec3 reflectionDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), shininess); // power 32 shininess value
    vec3 specular = light.specular * spec * vec3(texture(specularTex, TexCoords));

    vec3 ambient = vec3(texture(diffuseTex, TexCoords)) * light.ambient;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, sampler2D diffuseTex, sampler2D specularTex, float shininess)
{
    // ambient light
    vec3 ambient = vec3(texture(diffuseTex, TexCoords)) * light.ambient;

    vec3 lightDir = normalize(light.position - fragPos);

    // smoothening intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = clamp((theta - light.outerCutoff) / epsilon, 0., 1.);

    float diff = max(dot(normal, lightDir), 0.);
    vec3 diffuse = diff * vec3(texture(diffuseTex, TexCoords)) * light.diffuse;

    vec3 reflectionDir = reflect(-lightDir, normal);

    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), shininess); // power 32 shininess value
    vec3 specular = vec3(texture(specularTex, TexCoords)) * spec * light.specular;

    diffuse *= intensity;
    specular *= intensity;

    return ambient + diffuse + specular;
}
