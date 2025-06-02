#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "LibMath/Geometry3D.h"
#include "Raycast.h" 
#include "SceneGraph.h"
#include "CircleUI.h"

class Player
{
private:
	LibMath::Capsule collider;
	CameraFPS camera;

	LibMath::Vector3 position;

	
	float yVelocity = 0.0f;

	const float gravity = -9.8f;
	const float jumpStrength = 10.0f;
	float movementSpeed = 2.5f;
	bool EKeyAlreadyPressed = false;

public:
	LibMath::Vector3 velocity;
	bool isJumping = false;

	Player();
	void Update(float deltaTime);
	void move(float deltaTime);
	void processInput(GLFWwindow* window, float deltaTime, SceneGraph<GameObject>& scene, CircleUI& phone);
	void ProcessKeyboard(char direction, float deltaTime);
	void checkForwardRaycast(SceneGraph<GameObject>& scene, CircleUI& phone);

	CameraFPS& getCamera() { return camera; };
	LibMath::Vector3& getPosition() { return position; };
	LibMath::Capsule& getCapsule() { return collider; };
};