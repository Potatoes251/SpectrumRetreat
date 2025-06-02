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

    auto cubeModel = resourceManager.Create<Model>("../../Assets/Meshes/cube.obj");
    cubeModel->uploadToGPU();

    auto cubeModelShared = std::shared_ptr<Model>(cubeModel, [](Model*) {});

    // Create GameObject and SceneNode
    auto cubeGO = std::make_shared<GameObject>("Blue Lapinou", cubeModelShared);

    auto cubeNode = std::make_shared<SceneNode<GameObject>>(cubeGO, shader);
    cubeNode->localTransform = LibMath::Matrix4::createTranslation({ 0, 0, 0}) *
                                LibMath::Matrix4::createScale({ .5f, .5f, .5f});
    scene.addNode(cubeNode);

    auto floorModelShared = std::shared_ptr<Model>(cubeModel, [](Model*) {});

    // Create GameObject and SceneNode
    auto floorGO = std::make_shared<GameObject>("Floor", floorModelShared);

    auto floorNode = std::make_shared<SceneNode<GameObject>>(floorGO, shader);
    floorNode->localTransform = LibMath::Matrix4::createTranslation({ 0,-2,0 }) *
                                LibMath::Matrix4::createScale({ 50, 1, 50});
    scene.addNode(floorNode);


    auto ceilingModelShared = std::shared_ptr<Model>(cubeModel, [](Model*) {});

    // Create GameObject and SceneNode
    auto ceilingGO = std::make_shared<GameObject>("Floor2", ceilingModelShared);

    auto ceilingNode = std::make_shared<SceneNode<GameObject>>(ceilingGO, shader);
    ceilingNode->localTransform = LibMath::Matrix4::createTranslation({ 0,5,0 }) *
                                 LibMath::Matrix4::createScale({ 50, 1, 50 });
    scene.addNode(ceilingNode);
}

void GameManager::update(float deltaTime) 
{
    //player.Update(deltaTime);
    //checkWalls(player, scene);
    //player.move(deltaTime);
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
