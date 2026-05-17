#pragma once

#include <algorithm>  // for std::max
#include "../../LibMath/Header/LibMath/Vector.h"

struct Light
{
    LibMath::Vector4    ambient;
    LibMath::Vector4    diffuse;
    LibMath::Vector4    specular;
};


struct DirectionalLight 
{
    Light   base;

    LibMath::Vector3    direction;
};


struct PointLight : Light
{
    LibMath::Vector3 position;

    float   constant;
    float   linear;
    float   quadratic;
};


struct SpotLight : Light 
{
    LibMath::Vector3    position;
    LibMath::Vector3    direction;

    float   cutOff;
    float   outerCutOff;
    float   constant;
    float   linear;
    float   quadratic;
};

LibMath::Vector3 getRGB(const LibMath::Vector4& color);
LibMath::Vector3 CalcDirectionalLight(const DirectionalLight& light, const LibMath::Vector3& normal, const LibMath::Vector3& viewDir);
LibMath::Vector3 CalcPointLight(const PointLight& light, const LibMath::Vector3& normal, const LibMath::Vector3& fragPos, const LibMath::Vector3& viewDir);
LibMath::Vector3 CalcSpotLight(const SpotLight& light, const LibMath::Vector3& normal, const LibMath::Vector3& fragPos, const LibMath::Vector3& viewDir);