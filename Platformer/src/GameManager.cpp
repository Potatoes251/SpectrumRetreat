#include "GameManager.h"
#include "GLTexture.h"
#include <GLFW/glfw3.h>


GameManager::GameManager() {}
GameManager::~GameManager() {}

void GameManager::init(float windowWidth, float windowHeight)
{
    
    // Load shader
    shader = new Shader();
    shader->SetVertexShader("../../Assets/Shaders/vertex.glsl");
    shader->SetFragmentShader("../../Assets/Shaders/fragment.glsl");
    shader->Link();

    auto cubeModel = resourceManager.Create<Model>("../../Assets/Meshes/cube.obj");
    cubeModel->uploadToGPU();

    auto cubeModelShared = std::shared_ptr<Model>(cubeModel, [](Model*) {});

    // Create Cube
    auto cubeGO = std::make_shared<GameObject>("Cube", "interactable", cubeModelShared);
    cubeGO->color = LibMath::Vector4(0.5f, 0.0f, 0.5f, 1.0f);

    auto cubeNode = std::make_shared<SceneNode<GameObject>>(cubeGO, shader);
    cubeNode->localTransform = LibMath::Matrix4::createTranslation({ 0, 0, 0}) *
                                LibMath::Matrix4::createScale({ .5f, .5f, .5f});
    scene.addNode(cubeNode);

    
    // Create Door
    auto doorGO = std::make_shared<GameObject>("Door", cubeModelShared);
    doorGO->isDoor = true;
    doorGO->color = LibMath::Vector4(1.0f, 1.f, 1.f, 1.0f);

    auto doorNode = std::make_shared<SceneNode<GameObject>>(doorGO, shader);
    doorNode->localTransform = LibMath::Matrix4::createTranslation({ 0, 0, -2.5 }) *
        LibMath::Matrix4::createScale({ 2.5f, 2.5f, 2.5f });
    scene.addNode(doorNode);



    // Create Floor
    auto floorModelShared = std::shared_ptr<Model>(cubeModel, [](Model*) {});
    
    auto floorGO = std::make_shared<GameObject>("Floor", floorModelShared);
    floorGO->color = LibMath::Vector4(1.0f, 0.4f, 0.7f, 1.0f);

    auto floorNode = std::make_shared<SceneNode<GameObject>>(floorGO, shader);
    floorNode->localTransform = LibMath::Matrix4::createTranslation({ 0,-2,0 }) *
                                LibMath::Matrix4::createScale({ 50, 1, 50});

    scene.addNode(floorNode);


    // Create Ceiling
    auto ceilingModelShared = std::shared_ptr<Model>(cubeModel, [](Model*) {});
    
    auto ceilingGO = std::make_shared<GameObject>("Ceiling", ceilingModelShared);
    ceilingGO->color = LibMath::Vector4(0.68f, 0.85f, 0.90f, 1.0f);

    auto ceilingNode = std::make_shared<SceneNode<GameObject>>(ceilingGO, shader);
    ceilingNode->localTransform = LibMath::Matrix4::createTranslation({ 0,5,0 }) *
                                 LibMath::Matrix4::createScale({ 50, 1, 50 });

    scene.addNode(ceilingNode);


    // PHONE

    // Phone
    float phoneRadius = windowHeight / 10.0f;

    // Position from bottom-right (in pixels)
    float posX = windowWidth - phoneRadius - 20.0f;  // 20px margin
    float posY = windowHeight - phoneRadius - 20.0f;

    auto phone = std::make_unique<CircleUI>(
        LibMath::Vector2(posX, posY), phoneRadius, ImColor(255, 255, 255, 255)
    );

    phonePtr = phone.get();

    uiManager.addElement(std::move(phone));

    // Crosshair
    float crosshairRadius = 2;
    
    posX = windowWidth / 2; 
    posY = windowHeight / 2;

    auto crosshair = std::make_unique<CircleUI>(
        LibMath::Vector2(posX, posY), crosshairRadius, ImColor(255, 255, 255, 255)
    );

    crosshairPtr = crosshair.get();

    uiManager.addElement(std::move(crosshair));
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
