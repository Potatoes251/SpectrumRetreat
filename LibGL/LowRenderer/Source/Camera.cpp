#include "Camera.h"
#include "../../LibMath/Header/LibMath/Angle.h"

#include <algorithm>

LibMath::Matrix4 CameraFPS::GetViewMatrix() 
{
    return LibMath::Matrix4::lookAt(position, position + front, up);
}
LibMath::Matrix4 CameraFPS::GetProjectionMatrix(float aspectRatio)
{
    return LibMath::Matrix4::perspectiveProjection(fov.radian(), aspectRatio, 0.1f, 100.0f);
}

CameraFPS::CameraFPS(LibMath::Vector3 startPosition, LibMath::Vector3 startUp, float startYaw, float startPitch)
{
    position = startPosition;
    worldUp = startUp;
    yaw = LibMath::Degree(startYaw);
    pitch = LibMath::Degree(startPitch);
    fov = LibMath::Degree(90);
    mouseSensitivity = 0.4f;

    UpdateCameraVectors();
}

void CameraFPS::ProcessMouseMovement(float xoffset, float yoffset) 
{
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += LibMath::Degree(xoffset);
    pitch += LibMath::Degree(yoffset);

    // Pitch clamp
    if (pitch.raw() > 89.0f)
        pitch = LibMath::Degree(89.0f);

    if (pitch.raw() < -89.0f)
        pitch = LibMath::Degree(-89.0f);

    UpdateCameraVectors();
}

void CameraFPS::UpdateCameraVectors()
{
    float yawRad = yaw.radian();
    float pitchRad = pitch.radian();

    LibMath::Vector3 frontLocal;

    frontLocal.m_x = cos(yawRad) * cos(pitchRad);
    frontLocal.m_y = sin(pitchRad);
    frontLocal.m_z = sin(yawRad) * cos(pitchRad);

    front = frontLocal;
    front.normalize();

    right = front.cross(worldUp);
    right.normalize();
    up = right.cross(front);
    up.normalize();
}



float CameraFPS::GetSensitivity()
{
    return mouseSensitivity;
}


