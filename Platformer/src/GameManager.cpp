#include "../header/GameManager.h"
#include "GLTexture.h"
#include <GLFW/glfw3.h>

GameManager::GameManager() {}
GameManager::~GameManager() {}

void GameManager::init() 
{
    // Load shader
    shader = std::make_unique<Shader>();
    shader->SetVertexShader("../../Assets/Shaders/vertex.glsl");
    shader->SetFragmentShader("../../Assets/Shaders/fragment.glsl");
    shader->Link();

    // Load model
    //model = std::make_shared<Model>("../../Assets/Meshes/bunny.obj");
    //model->uploadToGPU();

    auto bunnyModel = resourceManager.Create<Model>("../../Assets/Meshes/bunny.obj");
    bunnyModel->uploadToGPU();

    //auto johnTex = resourceManager.Create<GLTexture>("../../Assets/Textures/john_smile.png");

    //shader->Use();

    //// Tell shader that the texture sampler "texture1" corresponds to texture unit 0
    //glUniform1i(glGetUniformLocation(shader->GetID(), "texture1"), 0);

    //if (johnTex)
    //{
    //    johnTex->bind(0);
    //}

    auto bunnyModelShared = std::shared_ptr<Model>(bunnyModel, [](Model*) {});

    // Create GameObject and SceneNode
    auto bunnyGO = std::make_shared<GameObject>("Lapinou", bunnyModelShared/*, johnTex*/);
    bunnyGO->position = LibMath::Vector3(0, 0, 0);

    auto bunnyNode = std::make_shared<SceneNode<GameObject>>(bunnyGO, shader.get());
    scene.addNode(bunnyNode);


    auto floorModel = resourceManager.Create<Model>("../../Assets/Meshes/floor.obj");
    floorModel->uploadToGPU();

    auto floorModelShared = std::shared_ptr<Model>(floorModel, [](Model*) {});

    // Create GameObject and SceneNode
    auto floorGO = std::make_shared<GameObject>("Sol", floorModelShared/*, johnTex*/);
    floorGO->position = LibMath::Vector3(0, 0, 0);

    auto floorNode = std::make_shared<SceneNode<GameObject>>(floorGO, shader.get());
    scene.addNode(floorNode);
}

void GameManager::update(float deltaTime) 
{
    // Update camera (if needed)
    camera.UpdateCameraVectors();

    // Update scene (logic + transform)
    scene.update(deltaTime);
}

void GameManager::draw()
{
    // Activate shader and set camera matrices
    LibMath::Matrix4 view = camera.GetViewMatrix();
    LibMath::Matrix4 projection = camera.GetProjectionMatrix((float)2560 / 1440);
    shader->Use();
    shader->setMat4("view", view);
    shader->setMat4("projection", projection);

    // Render scene graph
    scene.draw(shader.get());
}

void GameManager::processMouseMovement(double xpos, double ypos) {
    camera.ProcessMouseMovement(static_cast<float>(xpos), static_cast<float>(ypos));
}
