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

    auto roundCubeModel = resourceManager.Create<Model>("../../Assets/Meshes/rounded_cube.obj");
    roundCubeModel->uploadToGPU();

    auto roundCubeModelShared = std::shared_ptr<Model>(roundCubeModel, [](Model*) {});

    loadSceneFromFile("../../Assets/scene.txt", cubeModelShared, roundCubeModelShared);

    initUI(windowWidth, windowHeight);
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

void	GameManager::addDoorObject(const std::string& name,
    const std::shared_ptr<Model>& model,
    const LibMath::Vector4& color,
    const LibMath::Matrix4& transform)
{
    auto doorGO = std::make_shared<GameObject>(name, model);
    doorGO->isDoor = true;
    doorGO->color = color;

    auto doorNode = std::make_shared<SceneNode<GameObject>>(doorGO, shader);
    doorNode->localTransform = transform;

    scene.addNode(doorNode);
}

void GameManager::loadSceneFromFile(const std::string& path, 
    const std::shared_ptr<Model>& defaultModel, 
    const std::shared_ptr<Model>& roundedModel)
{
    std::ifstream file(path);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);

        std::string name, tag;
        float r, g, b, a;
        float tx, ty, tz;
        float sx, sy, sz;
        bool isDoor;

        iss >> name >> tag >> r >> g >> b >> a >> tx >> ty >> tz >> sx >> sy >> sz >> isDoor;

        LibMath::Vector4 color(r, g, b, a);
        LibMath::Matrix4 transform = LibMath::Matrix4::createTranslation({ tx, ty, tz }) *
            LibMath::Matrix4::createScale({ sx, sy, sz });

        std::shared_ptr<Model> model;
        if (tag == "interactable")
        {
            model = roundedModel;
        }
        else
        {
            model = defaultModel;
        }

        if (isDoor)
        {
            addDoorObject(name, model, color, transform);
        }
        else
        {
            addGameObject(name, tag, model, color, transform);
        }
    }
}

void    GameManager::update(float deltaTime) 
{
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
