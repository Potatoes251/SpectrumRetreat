#include "light.h"

inline LibMath::Vector3 getRGB(const LibMath::Vector4& color)
{
    return LibMath::Vector3(color.m_x, color.m_y, color.m_z);
}

LibMath::Vector3 CalcDirectionalLight(const DirectionalLight& light, const LibMath::Vector3& normal, const LibMath::Vector3& viewDir)
{
    LibMath::Vector3 lightDir = -light.direction;

    lightDir.normalize();

    float diff = std::max(normal.dot(lightDir), 0.0f);

    LibMath::Vector3 halfwayDir = lightDir + viewDir;

    halfwayDir.normalize();

    float spec = pow(std::max(normal.dot(halfwayDir), 0.0f), 32.0f);

    LibMath::Vector3 ambient = getRGB(light.base.ambient);
    LibMath::Vector3 diffuse = getRGB(light.base.diffuse) * diff;
    LibMath::Vector3 specular = getRGB(light.base.specular) * spec;

    return ambient + diffuse + specular;
}

LibMath::Vector3 CalcPointLight(const PointLight& light, const LibMath::Vector3& normal, const LibMath::Vector3& fragPos, const LibMath::Vector3& viewDir)
{
    LibMath::Vector3 lightDir = light.position - fragPos;

    lightDir.normalize();

    float diff = std::max(normal.dot(lightDir), 0.0f);

    LibMath::Vector3 halfwayDir = lightDir + viewDir;

    halfwayDir.normalize();

    float spec = pow(std::max(normal.dot(halfwayDir), 0.0f), 32.0f);

    float distance = (light.position - fragPos).magnitude();
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    LibMath::Vector3 ambient = getRGB(light.ambient);
    LibMath::Vector3 diffuse = getRGB(light.diffuse) * diff;
    LibMath::Vector3 specular = getRGB(light.specular) * spec;

    return (ambient + diffuse + specular) * attenuation;
}

LibMath::Vector3 CalcSpotLight(const SpotLight& light, const LibMath::Vector3& normal, const LibMath::Vector3& fragPos, const LibMath::Vector3& viewDir)
{
    LibMath::Vector3 lightDir = light.position - fragPos;

    lightDir.normalize();

    LibMath::Vector3 directionLight = -light.direction;

    directionLight.normalize();

    float theta = (lightDir.dot(directionLight));


    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = std::clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

    float diff = std::max(normal.dot(lightDir), 0.0f);

    LibMath::Vector3 halfwayDir = (lightDir + viewDir);

    halfwayDir.normalize();

    float spec = pow(std::max(normal.dot(halfwayDir), 0.0f), 32.0f);

    float distance = (light.position - fragPos).magnitude();
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    LibMath::Vector3 ambient = getRGB(light.ambient);
    LibMath::Vector3 diffuse = getRGB(light.diffuse) * diff;
    LibMath::Vector3 specular = getRGB(light.specular) * spec;

    return (ambient + diffuse + specular) * attenuation * intensity;
}