#pragma once

#include "../../LibMath/Header/LibMath/Vector.h"
#include "../../LibMath/Header/LibMath/Matrix.h"

class CameraFPS
{
public:
    LibMath::Vector3    position;
    LibMath::Vector3    front;
    LibMath::Vector3    up;
    LibMath::Vector3    right;
    LibMath::Vector3    worldUp;

    LibMath::Degree     yaw;
    LibMath::Degree     pitch;
    LibMath::Degree     fov;

    float   mouseSensitivity;

    CameraFPS(
        LibMath::Vector3 startPosition,
        LibMath::Vector3 startUp,
        float startYaw = 0.0f,
        float startPitch = 0.0f
    );

    LibMath::Matrix4    GetViewMatrix();
    LibMath::Matrix4    GetProjectionMatrix(float aspectRatio);

    void    ProcessMouseMovement(float xoffset, float yoffset);
    float   GetSensitivity();

private:
    void    UpdateCameraVectors();
};