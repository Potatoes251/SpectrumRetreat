#include "../header/GameManager.h"
#include <GLFW/glfw3.h>

//GameManager::GameManager()
//	: /*player(nullptr), scene(nullptr),*/ isRunning(true) {}
//
//GameManager::~GameManager() {}
//
//void GameManager::init() 
//{
//    Model* bunny = new Model("../../Assets/Meshes/bunny.obj");
//    auto bunnyGO = std::make_shared<GameObject>("Lapinou", bunny);
//    bunnyGO->position = LibMath::Vector3(0, 0, 0);
//
//    auto node = std::make_shared<SceneNode<GameObject>>(bunnyGO.get(), shaderPtr);
//    scene.addNode(node);
//
//    //scene.add(bunnyGO);
//}
////
////void GameManager::run() 
////{
////    while (isRunning && !glfwWindowShouldClose(glfwGetCurrentContext())) 
////    {
////        processInput();
////        update();
////        render();
////    }
////}
//
//void GameManager::processInput()
//{
//}
//
//void GameManager::update(float deltaTime)
//{
//    //float deltaTime = glfwGetTime() - lastFrame;
//    //lastFrame = glfwGetTime();
//
//    scene.update(deltaTime);
//}
//
////void GameManager::render()
////{
////    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
////    shader->Use();
////
////    scene.draw(shader); // Chaque GameObject fait son propre draw
////    glfwSwapBuffers(glfwGetCurrentContext());
////}


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

    auto bunnyModelShared = std::shared_ptr<Model>(bunnyModel, [](Model*) {});

    // Create GameObject and SceneNode
    auto bunnyGO = std::make_shared<GameObject>("Bunny", bunnyModelShared);
    bunnyGO->position = LibMath::Vector3(0, 0, 0);

    auto bunnyNode = std::make_shared<SceneNode<GameObject>>(bunnyGO, shader.get());
    scene.addNode(bunnyNode);

    //scene.
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
