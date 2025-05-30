#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "LibMath/Geometry3D.h"

class Player
{
private:
	LibMath::Capsule collider;
	CameraFPS camera;

	LibMath::Vector3 position;

	LibMath::Vector3 velocity;
	float yVelocity = 0.0f;
	bool isJumping = false;

	const float gravity = -9.8f;
	const float jumpStrength = 5.0f;
	float movementSpeed = 2.5f;
public:
	Player();
	void Update(float deltaTime);
	void move(LibMath::Vector3 movement);
	void processInput(GLFWwindow* window, float deltaTime);
	void ProcessKeyboard(char direction, float deltaTime);

	CameraFPS& getCamera() { return camera; };
	LibMath::Vector3& getPosition() { return position; };
	LibMath::Capsule& getCapsule() { return collider; };
};