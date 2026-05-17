#pragma once

#include "SceneGraph.h"
#include "GameObject.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "Player.h"
#include "Physics.h"
#include "UIManager.h"
#include "ButtonUI.h"
#include "SoundManager.h"

class Application;

#include <../../External/irrKlang/include/irrKlang.h>

class GameManager 
{
public:
    GameManager() = default;
    ~GameManager();

    // Initialize resources and scene
    void    init(float windowWidth, float windowHeight);
    void    loadSceneFromFile(const std::string& path, 
        const std::shared_ptr<Model>& defaultModel, 
        const std::shared_ptr<Model>& roundedModel);

    // initUI
    void    initUI(float windowWidth, float windowHeight);

    // add a GO to the scene
    void	addGameObject(const std::string& name,
        const std::string& tag,
        const std::shared_ptr<Model>& model,
        const LibMath::Vector4& color,
        const LibMath::Matrix4& transformMatrix);

    // reference the application in gameManager
    void    setApplication(Application* app);

    // reset the gameManager
    void    restart(float windowWidth, float windowHeight);

    // Update logic (scene, camera) each frame
    void    update(float deltaTime);
    void    updateMovingObjects();

    // Render the scene
    void    draw();

    // Handle mouse movement for camera
    void    processMouseMovement(double xpos, double ypos);

    SceneGraph<GameObject>  scene;

    Player      player;

    Shader*     shader = nullptr;

    // UI
    UIManager   uiManager;

    CircleUI*   phonePtr = nullptr; 
    CircleUI*   crosshairPtr = nullptr;

    ButtonUI*   resumeButton = nullptr;
    ButtonUI*   restartButton = nullptr;
    ButtonUI*   quitButton = nullptr;

    // Sound
    SoundManager    soundManager;
    float           totalTime = 0.0f;

    // Lighting
    std::vector<PointLight> pointLights;
    std::vector<SpotLight>  spotLights;

private:
    ResourceManager         resourceManager;
    Application*            application = nullptr;
    bool                    firstLoad = true;
};