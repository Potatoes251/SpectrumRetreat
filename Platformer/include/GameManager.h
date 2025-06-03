#pragma once

#include "SceneGraph.h"
#include "GameObject.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "Player.h"
#include "Physics.h"
#include "UIManager.h"

class GameManager 
{
public:
    GameManager();
    ~GameManager();

    // Initialize resources and scene
    void init(float windowWidth, float windowHeight);

    // Update logic (scene, camera) each frame
    void update(float deltaTime);

    // Render the scene
    void draw();

    // Handle mouse movement for camera
    void processMouseMovement(double xpos, double ypos);

    SceneGraph<GameObject> scene;
    Player player;
    Shader* shader;
    UIManager uiManager;
    CircleUI* phonePtr = nullptr; 
    CircleUI* crosshairPtr = nullptr;
private:
    ResourceManager resourceManager;
};