#version 330 core

#define MAX_POINT_LIGHTS 8

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    Light base;
    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec4 objectColor;

out vec4 FragColor;
in vec4 vertexColor;

uniform sampler2D texture1;
uniform bool useTexture;

uniform vec3 viewPos;
uniform int numPointLights;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64.0f); 

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = light.base.ambient;
    vec3 diffuse = light.base.diffuse * diff;
    vec3 specular = light.base.specular * spec;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 lighting = vec3(0.0);

    for (int i = 0; i < numPointLights; i++)
    {
        lighting += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

//    if (numPointLights > 0)
//    {
//        lighting /= float(numPointLights);
//    }

    vec4 baseColor = useTexture ? texture(texture1, TexCoords) : objectColor;

    FragColor = vec4(lighting, 1.0) * baseColor;
}