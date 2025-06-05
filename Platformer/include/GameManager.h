#pragma once

#include "SceneGraph.h"
#include "GameObject.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "Player.h"
#include "Physics.h"
#include "UIManager.h"

#include <../../External/irrKlang/include/irrKlang.h>

class GameManager 
{
public:
    GameManager();
    ~GameManager();

    // Initialize resources and scene
    void    init(float windowWidth, float windowHeight);

    // initUI
    void initUI(float windowWidth, float windowHeight);

    // add a GO to the scene
    void	addGameObject(const std::string& name,
        const std::string& tag,
        const std::shared_ptr<Model>& model,
        const LibMath::Vector4& color,
        const LibMath::Matrix4& transformMatrix);

    // Update logic (scene, camera) each frame
    void    update(float deltaTime);

    // Render the scene
    void    draw();

    // Handle mouse movement for camera
    void    processMouseMovement(double xpos, double ypos);

    SceneGraph<GameObject>  scene;
    Player      player;
    Shader*     shader;
    UIManager   uiManager;
    CircleUI*   phonePtr = nullptr; 
    CircleUI*   crosshairPtr = nullptr;
    irrklang::ISoundEngine* engine;

private:
    ResourceManager     resourceManager;
};