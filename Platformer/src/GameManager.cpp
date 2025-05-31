#include "GameManager.h"
#include "GLTexture.h"
#include <GLFW/glfw3.h>

GameManager::GameManager() {}
GameManager::~GameManager() {}

void GameManager::init() 
{
    // Load shader
    shader = new Shader();
    shader->SetVertexShader("../../Assets/Shaders/vertex.glsl");
    shader->SetFragmentShader("../../Assets/Shaders/fragment.glsl");
    shader->Link();

    auto bunnyModel = resourceManager.Create<Model>("../../Assets/Meshes/bunny.obj");
    bunnyModel->uploadToGPU();

    auto bunnyModelShared = std::shared_ptr<Model>(bunnyModel, [](Model*) {});

    // Create GameObject and SceneNode
    auto bunnyGO = std::make_shared<GameObject>("Blue Lapinou", bunnyModelShared);

    auto bunnyNode = std::make_shared<SceneNode<GameObject>>(bunnyGO, shader);
    scene.addNode(bunnyNode);


    auto floorModel = resourceManager.Create<Model>("../../Assets/Meshes/floor.obj");
    floorModel->uploadToGPU();

    auto floorModelShared = std::shared_ptr<Model>(floorModel, [](Model*) {});

    // Create GameObject and SceneNode
    auto floorGO = std::make_shared<GameObject>("Floor", floorModelShared);

    auto floorNode = std::make_shared<SceneNode<GameObject>>(floorGO, shader);
    scene.addNode(floorNode);
}

void GameManager::update(float deltaTime) 
{
    player.Update(deltaTime);

    // Update scene (logic + transform)
    scene.update();
}

void GameManager::draw()
{
    // Activate shader and set camera matrices
    LibMath::Matrix4 view = player.getCamera().GetViewMatrix();
    LibMath::Matrix4 projection = player.getCamera().GetProjectionMatrix((float)2560 / 1440);
    shader->Use();
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    // Render scene graph
    scene.draw(shader);
}

void GameManager::processMouseMovement(double xpos, double ypos) 
{
    player.getCamera().ProcessMouseMovement(static_cast<float>(xpos), static_cast<float>(ypos));
}
