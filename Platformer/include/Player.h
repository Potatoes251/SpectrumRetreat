#pragma once

#include "Camera.h"
#include "LibMath/Geometry3D.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
class Player
{
private:
	LibMath::Capsule collider;
	CameraFPS camera;

	float yVelocity = 0.0f;
	bool isJumping = false;

	const float gravity = -9.8f;
	const float jumpStrength = 5.0f;
	float movementSpeed = 2.5f;
public:
	Player();
	void Update(float deltaTime);
	void processInput(GLFWwindow* window, float deltaTime);
	void ProcessKeyboard(char direction, float deltaTime);

	CameraFPS& getCamera() { return camera; };
};