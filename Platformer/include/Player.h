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

class GameManager;

class Player
{
private:
	LibMath::Capsule	collider;
	CameraFPS			camera;

	LibMath::Vector3	position;

	
	float	yVelocity = 0.0f;

	const float		gravity = -9.8f;
	const float		jumpStrength = 7.5f;
	float	movementSpeed = 10.f;
	bool	EKeyAlreadyPressed = false;
	GameManager*	gameManager = nullptr;

public:
	LibMath::Vector3	velocity;
	bool		isJumping = false;

	Player();

	// change the velocity
	void	update(float deltaTime);

	// move the player using the velocity
	void	move(float deltaTime);

	void	processInput(GLFWwindow* window, float deltaTime, SceneGraph<GameObject>& scene, CircleUI& phone, CircleUI& crosshair);
	void	checkForwardRaycast(GLFWwindow* window);

	// set the reference to the gameManager
	void	setGameManager(GameManager* gm) { gameManager = gm; }

	// getters
	CameraFPS&			getCamera() { return camera; };
	LibMath::Vector3&	getPosition() { return position; };
	LibMath::Capsule&	getCapsule() { return collider; };
};