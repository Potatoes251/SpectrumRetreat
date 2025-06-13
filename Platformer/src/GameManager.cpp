#include "GameManager.h"
#include "Application.h"
#include "GLTexture.h"
#include <GLFW/glfw3.h>


GameManager::~GameManager()
{
    if (shader)
    {
        delete shader;

        shader = nullptr;
    }

    resumeButton = nullptr;
    restartButton = nullptr;
    quitButton = nullptr;

    phonePtr = nullptr;
    crosshairPtr = nullptr;

    scene.clear();
}

void    GameManager::init(float windowWidth, float windowHeight)
{
    // Load shader
    if (shader == nullptr)
    {
        shader = new Shader();
        shader->SetVertexShader("../../Assets/Shaders/vertex.glsl");
        shader->SetFragmentShader("../../Assets/Shaders/fragment.glsl");
        shader->Link();
    }

    std::shared_ptr<Model> cubeModelShared;
    std::shared_ptr<Model> roundCubeModelShared;

    if (firstLoad)
    {
        // create the assets if first time loading
        auto cubeModel = resourceManager.Create<Model>("../../Assets/Meshes/cube.obj");

        cubeModelShared = std::shared_ptr<Model>(cubeModel, [](Model*) {});
        cubeModelShared -> uploadToGPU();

        auto roundCubeModel = resourceManager.Create<Model>("../../Assets/Meshes/rounded_cube.obj");
        roundCubeModel -> uploadToGPU();

        roundCubeModelShared = std::shared_ptr<Model>(roundCubeModel, [](Model*) {});

        // init the ui elements
        initUI(windowWidth, windowHeight);

        // start music at the beginning of the game
        soundManager.playMusic("../../Assets/Sounds/calmPiano.ogg", true);
        soundManager.setMusicVolume(0.6f);
        firstLoad = false;
    }
    else
    {
        // get the assets if restarting
        auto cubeModel = resourceManager.Get<Model>("../../Assets/Meshes/cube.obj");

        cubeModelShared = std::shared_ptr<Model>(cubeModel, [](Model*) {});

        auto roundCubeModel = resourceManager.Get<Model>("../../Assets/Meshes/rounded_cube.obj");

        roundCubeModelShared = std::shared_ptr<Model>(roundCubeModel, [](Model*) {});

        phonePtr -> setColor(ImColor(255, 255, 255, 255));
    }
    // reference the game manager in the player
    player.setGameManager(this);

    // load the scene from the file
    loadSceneFromFile("../../Assets/scene.txt", cubeModelShared, roundCubeModelShared);
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
    float buttonWidth = windowWidth * 0.25f;   // 25% of screen width
    float buttonHeight = windowHeight * 0.07f; // 7% of screen height
    float spacing = windowHeight * 0.015f;     // 1.5% of screen height

    // Compute base X and Y to center the buttons
    float baseX = (float)windowWidth / 2.0f - buttonWidth / 2.0f;
    float totalHeight = 3 * buttonHeight + 2 * spacing;
    float baseY = (float)windowHeight / 2.0f - totalHeight / 2.0f;

    // create resume Button
    resumeButton = new ButtonUI(
        "Resume",                                 // name
        ImVec2(baseX, baseY),                     // coordinate
        ImVec2(buttonWidth, buttonHeight),        // size
        [this]() { application -> exitPause(); }  // function pointer (for when the button is clicked)
    );
    resumeButton -> m_isActive = false;           // Start inactive
    uiManager.addElement(std::unique_ptr<ButtonUI>(resumeButton));

    // create restart Button
    restartButton = new ButtonUI(
        "Restart",
        ImVec2(baseX, baseY + buttonHeight + spacing),
        ImVec2(buttonWidth, buttonHeight),
        [this]() { application -> restartGame(); }
    );
    restartButton -> m_isActive = false;
    uiManager.addElement(std::unique_ptr<ButtonUI>(restartButton));

    // create quit Button
    quitButton = new ButtonUI(
        "Quit",
        ImVec2(baseX, baseY + 2 * (buttonHeight + spacing)),
        ImVec2(buttonWidth, buttonHeight),
        [this]() { application -> quitGame(); }
    );
    quitButton -> m_isActive = false; // Start inactive

    uiManager.addElement(std::unique_ptr<ButtonUI>(quitButton));
}

void	GameManager::addGameObject(const std::string& name,
    const std::string& tag,
    const std::shared_ptr<Model>& model,
    const LibMath::Vector4& color,
    const LibMath::Matrix4& transformMatrix)
{
    auto gameObject = std::make_shared<GameObject>(name, tag, model);
    gameObject -> m_color = color;

    auto node = std::make_shared<SceneNode<GameObject>>(gameObject, shader);
    node -> localTransform = transformMatrix;

    scene.addNode(node);
}

void    GameManager::loadSceneFromFile(const std::string& path, 
    const std::shared_ptr<Model>& defaultModel, 
    const std::shared_ptr<Model>& roundedModel)
{
    // open the file
    std::ifstream file(path);

    if (!file.is_open()) return;

    // analyze the file line per line
    std::string line;

    while (std::getline(file, line))
    {
        // skip empty or commented lines
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);

        // create variable to store the data in the line
        std::string name, tag;
        float r, g, b, a;
        float tx, ty, tz;
        float sx, sy, sz;

        // extract data from the line
        iss >> name >> tag >> r >> g >> b >> a >> tx >> ty >> tz >> sx >> sy >> sz;

        LibMath::Vector4 color(r, g, b, a);
        LibMath::Matrix4 transform = LibMath::Matrix4::createTranslation({ tx, ty, tz }) *
            LibMath::Matrix4::createScale({ sx, sy, sz });

        // give a different model to the objects with the tag "interactable"
        std::shared_ptr<Model> model;
        if (tag == "interactable")
        {
            model = roundedModel;
        }
        else
        {
            model = defaultModel;
        }

        // create the gameObject
        addGameObject(name, tag, model, color, transform);
    }
}

void    GameManager::setApplication(Application* app)
{
    application = app;
}

void    GameManager::restart(float windowWidth, float windowHeight)
{
    // reset the game Manager
    player.~Player();
    new (&player) Player();
    scene.clear();
    init(windowWidth, windowHeight);
}

void    GameManager::update(float deltaTime) 
{
    // Update scene (transforms)
    scene.update();
    updateMovingObjects();

}

void GameManager::updateMovingObjects()
{
    float amplitude = 2.0f;
    float speed = 1.0f;
    float offset = amplitude * std::sin(totalTime * speed);

    LibMath::Vector3 direction = { 1.0f, 0.0f, 0.0f };
    LibMath::Vector3 startPosition = { 0.0f, 7.5f, -20.0f };
    LibMath::Vector3 newPos = startPosition + direction * offset;

    std::shared_ptr<SceneNode<GameObject>> node = scene.findNodeByName("Cube3");

    if (node != nullptr)
    {
        node->localTransform = LibMath::Matrix4::createTransform(newPos, { 0, 0, 0 }, LibMath::Radian(0), {0.5f, 0.5f, 0.5f});

        node->markDirty();
    }
}

void    GameManager::draw()
{
    // Activate shader and set camera matrices
    LibMath::Matrix4 view = player.getCamera().GetViewMatrix();
    LibMath::Matrix4 projection = player.getCamera().GetProjectionMatrix((float)2560 / 1440);

    shader -> Use();
    shader -> setMat4("view", view);
    shader -> setMat4("projection", projection);

    // Render scene graph
    scene.draw(shader);
}

void    GameManager::processMouseMovement(double xpos, double ypos) 
{
    player.getCamera().ProcessMouseMovement(static_cast<float>(xpos), static_cast<float>(ypos));
}
