#include "GameManager.h"
#include "Application.h"
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

    // Add game objects

    // cube that can be used to change phone color
    addGameObject("Cube", "interactable", cubeModelShared,
        { 0.86f, 0.08f, 0.24f, 1.0f },
        LibMath::Matrix4::createTranslation({ 2, 0, 0 }) *
        LibMath::Matrix4::createScale({ 0.5f, 0.5f, 0.5f }));

    addGameObject("Cube2", "interactable", cubeModelShared,
        { 0.71f, 0.49f, 0.86f, 1.0f },
        LibMath::Matrix4::createTranslation({ 0, 0, -20 }) *
        LibMath::Matrix4::createScale({ 0.5f, 0.5f, 0.5f }));

    addGameObject("Cube3", "interactable", cubeModelShared,
        { 0.71f, 0.49f, 0.86f, 1.0f },
        LibMath::Matrix4::createTranslation({ 0, 6, -40 }) *
        LibMath::Matrix4::createScale({ 0.5f, 0.5f, 0.5f }));

    addGameObject("Cube4", "interactable", cubeModelShared,
        { 1.0f, 0.85f, 0.73f, 1.0f },
        LibMath::Matrix4::createTranslation({ 0, 6, -85 }) *
        LibMath::Matrix4::createScale({ 0.5f, 0.5f, 0.5f }));

    // add the floor & walls
    addGameObject("Floor", "", cubeModelShared,
        { 1.0f, 0.4f, 0.7f, 1.0f },
        LibMath::Matrix4::createTranslation({ 0, -1.5f, 0 }) *
        LibMath::Matrix4::createScale({ 5.0f, 1.0f, 50.0f }));

    addGameObject("Wall", "", cubeModelShared,
        { 1.0f, 1.f, 1.f, 1.0f },
        LibMath::Matrix4::createTranslation({ 0 , 0, 5 }) *
        LibMath::Matrix4::createScale({ 50.0f, 10.0f, 10.0f }));

    addGameObject("Wall2", "", cubeModelShared,
        { 1.0f, 1.f, 1.f, 1.0f },
        LibMath::Matrix4::createTranslation({ 0 , 0, -5 }) *
        LibMath::Matrix4::createScale({ 50.0f, 2.5f, 10.0f }));

    addGameObject("Wall3", "", cubeModelShared,
        { 1.0f, 1.f, 1.f, 1.0f },
        LibMath::Matrix4::createTranslation({ 5 , 0, 0 }) *
        LibMath::Matrix4::createScale({ 1.0f, 10.0f, 50.0f }));

    addGameObject("Wall4", "", cubeModelShared,
        { 1.0f, 1.f, 1.f, 1.0f },
        LibMath::Matrix4::createTranslation({ -5 , 0, 0 }) *
        LibMath::Matrix4::createScale({ 1.0f, 10.0f, 50.0f }));

    addGameObject("Wall5", "", cubeModelShared,
        { 0.71f, 0.49f, 0.86f, 1.0f },
        LibMath::Matrix4::createTranslation({ 0 , 0, -2.5f }) *
        LibMath::Matrix4::createScale({ 5.0f, 2.5f, 10.f }));

    //addGameObject("Wall6", "", cubeModelShared,
    //    { 0.71f, 0.49f, 0.86f, 1.0f },
    //    LibMath::Matrix4::createTranslation({ 0 , 0, -2.5f }) *
    //    LibMath::Matrix4::createScale({ 5.0f, 2.5f, 10.f }));


    addGameObject("Ceiling", "", cubeModelShared,
        { 0.68f, 0.85f, 0.90f, 1.0f },
        LibMath::Matrix4::createTranslation({ 0, 10, 0 }) *
        LibMath::Matrix4::createScale({ 5.0f, 1.0f, 50.0f }));


    // Create Door
    auto doorGO = std::make_shared<GameObject>("Door", cubeModelShared);
    doorGO->isDoor = true;
    doorGO->color = LibMath::Vector4(0.86f, 0.08f, 0.24f, 1.0f);

    auto doorNode = std::make_shared<SceneNode<GameObject>>(doorGO, shader);
    doorNode->localTransform = LibMath::Matrix4::createTranslation({ 0, 0, -10 }) *
                                LibMath::Matrix4::createScale({ 5.f, 2.5f, .5f });

    scene.addNode(doorNode);

    initUI(windowWidth, windowHeight);
    soundManager.playMusic("../../Assets/Sounds/fragments of hope.ogg", true);

    player.setGameManager(this);
}

void	GameManager::initUI(float windowWidth, float windowHeight)
{
    // PHONE
    float phoneRadius = windowHeight / 10.0f;
    float phonePosX = windowWidth - phoneRadius - 20.0f;
    float phonePosY = windowHeight - phoneRadius - 20.0f;

    auto phone = std::make_unique<CircleUI>(
        LibMath::Vector2(phonePosX, phonePosY),
        phoneRadius,
        ImColor(255, 255, 255, 255)
    );

    phonePtr = phone.get();
    uiManager.addElement(std::move(phone));

    // CROSSHAIR
    float crosshairRadius = 2.0f;
    float crosshairPosX = windowWidth / 2.0f;
    float crosshairPosY = windowHeight / 2.0f;

    auto crosshair = std::make_unique<CircleUI>(
        LibMath::Vector2(crosshairPosX, crosshairPosY),
        crosshairRadius,
        ImColor(255, 255, 255, 255)
    );

    crosshairPtr = crosshair.get();
    uiManager.addElement(std::move(crosshair));

    // Get button dimensions
    float buttonWidth = 200.0f;
    float buttonHeight = 50.0f;
    float spacing = 10.0f;

    // Compute base X and Y to center the buttons
    float baseX = (float)windowWidth / 2.0f - buttonWidth / 2.0f;
    float totalHeight = 3 * buttonHeight + 2 * spacing;
    float baseY = (float)windowHeight / 2.0f - totalHeight / 2.0f;

    resumeButton = new ButtonUI(
        "Resume",
        ImVec2(baseX, baseY),
        ImVec2(buttonWidth, buttonHeight),
        [this]() { application->exitPause(); }
    );
    resumeButton->isActive = false; // Start inactive
    uiManager.addElement(std::unique_ptr<ButtonUI>(resumeButton));

    restartButton = new ButtonUI(
        "Restart",
        ImVec2(baseX, baseY + buttonHeight + spacing),
        ImVec2(buttonWidth, buttonHeight),
        [this]() { application->exitPause(); }
    );
    restartButton->isActive = false;
    uiManager.addElement(std::unique_ptr<ButtonUI>(restartButton));

    quitButton = new ButtonUI(
        "Quit",
        ImVec2(baseX, baseY + 2 * (buttonHeight + spacing)),
        ImVec2(buttonWidth, buttonHeight),
        [this]() {
            if (application)
            {
                glfwSetWindowShouldClose(application->getWindow(), true);
            }
        }
    );
    quitButton->isActive = false;
    uiManager.addElement(std::unique_ptr<ButtonUI>(quitButton));
}

void	GameManager::addGameObject(const std::string& name,
    const std::string& tag,
    const std::shared_ptr<Model>& model,
    const LibMath::Vector4& color,
    const LibMath::Matrix4& transformMatrix)
{
    auto gameObject = std::make_shared<GameObject>(name, tag, model);
    gameObject->color = color;

    auto node = std::make_shared<SceneNode<GameObject>>(gameObject, shader);
    node->localTransform = transformMatrix;

    scene.addNode(node);
}

void GameManager::setApplication(Application* app)
{
    application = app;
}

void    GameManager::update(float deltaTime) 
{
    //player.Update(deltaTime);
    //checkWalls(player, scene);
    //player.move(deltaTime);
    // Update scene (logic + transform)
    scene.update();
}

void    GameManager::draw()
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

void    GameManager::processMouseMovement(double xpos, double ypos) 
{
    player.getCamera().ProcessMouseMovement(static_cast<float>(xpos), static_cast<float>(ypos));
}
