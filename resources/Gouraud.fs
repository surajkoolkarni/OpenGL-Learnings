#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightDir;
in vec3 Specular;
in vec3 LightColor;

uniform vec3 objectColor;

void main()
{
    // ambient light
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * LightColor;

    // diffuse light
    vec3 norm = normalize(Normal);

    float diff = max(dot(norm, LightDir), 0.);
    vec3 diffuse = diff * LightColor;

    vec3 result = (ambient + diffuse + Specular) * objectColor;

    FragColor = vec4(result, 1.0f);
}
