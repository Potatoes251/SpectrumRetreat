#include "Application.h"
#include "GLTexture.h"
#include "Player.h"

bool Application::init() 
{
    if (!initGLFW()) return false;
    if (!createWindow()) return false;
    if (!initGLAD()) return false;
    initGLContext();
    initImGuiContext();

    // enable alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize scene and game objects
    gameManager.init(windowWidth, windowHeight);

    if (!initShaderAndResources())
        return false;

    return true;
}

bool Application::initGLFW() 
{
    if (!glfwInit()) 
    {
        std::cerr << "Failed to initialize GLFW\n";

        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    return true;
}

void Application::initImGuiContext()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImGuiIO& io = ImGui::GetIO();

    gameManager.uiManager.fontSmall = io.Fonts->AddFontFromFileTTF("../../Assets/Fonts/Play-Regular.ttf", 16.0f);
    gameManager.uiManager.fontMedium = io.Fonts->AddFontFromFileTTF("../../Assets/Fonts/Play-Regular.ttf", 22.0f);
    gameManager.uiManager.fontLarge = io.Fonts->AddFontFromFileTTF("../../Assets/Fonts/Play-Regular.ttf", 32.0f);
}

bool Application::createWindow() 
{
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primary);

    windowWidth = 1920;
    windowHeight = 1200;

    window = glfwCreateWindow(windowWidth, windowHeight, "3D Game", nullptr, nullptr);

    if (!window) 
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    return true;
}

bool Application::initGLAD() 
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        std::cerr << "Failed to initialize GLAD\n";

        return false;
    }

    return true;
}

void Application::initGLContext() 
{
    //Set callbacks
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, this);

    glfwSwapInterval(0);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    // Register mouse movement callback function
    glfwSetCursorPosCallback(window, mouse_callback);
}

bool Application::initShaderAndResources()
{
    if (!setupResources()) // Load shaders, model, textures, set lighting and OpenGL states
    {
        std::cerr << "Failed to set up resources!\n";

        return false;
    }
    
    return true;
}

void Application::run() 
{
    while (!glfwWindowShouldClose(window)) 
    {
        updatePause();
        updateDeltaTime();

        if (!isPaused)
        {
            gameManager.player.Update(deltaTime);
            checkWalls(gameManager.player, gameManager.scene, gameManager.phonePtr->getColor());
            gameManager.player.move(deltaTime);
            gameManager.player.processInput(window, deltaTime, gameManager.scene, *gameManager.phonePtr, *gameManager.crosshairPtr);

            // Update game logic
            gameManager.update(deltaTime);
        }

        // Render
        clearScreen();
        renderFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void Application::updateDeltaTime() 
{
    float currentFrame = static_cast<float>(glfwGetTime());

    if (!isPaused)
    {
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
    }

    else
    {
        deltaTime = 0.0f;
        lastFrame = currentFrame;
    }
}

void Application::updateSceneGraph()
{
    updateDeltaTime();
    float angle = deltaTime * LibMath::Radian(LibMath::Degree(45.0f).radian()).raw();

    auto bunnyNode = gameManager.scene.findNodeByName("Blue Lapinou");
    
    if (bunnyNode)
    {
        bunnyNode->localTransform =
            LibMath::Matrix4::createTranslation({ 0, 5, 0 }) *
            LibMath::Matrix4::createRotationY(angle) *
            LibMath::Matrix4::createScale({ 0.5f,0.5f,0.5f });
        bunnyNode->object->color = LibMath::Vector4(0, 0, 1, 1);
    }

    auto floorNode = gameManager.scene.findNodeByName("Floor");

    if (floorNode)
    {
        floorNode->object->color = LibMath::Vector4(1, 0, 0, 1);
    }

    gameManager.scene.root->updateTransform();
}


void Application::clearScreen() 
{
    glClearColor(1.f, 1.f, 1.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Application::drawUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    gameManager.uiManager.drawAll();

    if (isPaused) // screen darkening in pause
    {
        int menuSizeX = windowWidth * 0.25;
        int menuSizeY = windowHeight * 0.4;
        int halfWindowWidth = windowWidth / 2.0f;
        int halfWindowHeight = windowHeight / 2.0f;

        int buttonWidth = 350;
        int buttonHeight = 100;
        int buttonPosX = (menuSizeX - buttonWidth) / 2;

        ImGui::GetBackgroundDrawList()->AddRectFilled(
            ImVec2(0, 0),
            ImVec2((float)windowWidth, (float)windowHeight),
            IM_COL32(0, 0, 0, 150)
        );

        ImGui::SetNextWindowSize(ImVec2(menuSizeX, menuSizeY));
        ImGui::SetNextWindowPos(ImVec2(halfWindowWidth - menuSizeX / 2, halfWindowHeight - menuSizeY / 2));

        ImGui::Begin("Pause Menu", nullptr,
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoTitleBar);

        ImGui::PushFont(gameManager.uiManager.fontLarge);

        ImGui::Spacing();
        ImGui::Spacing();

        const char* text = "Pause Menu";
        ImVec2 textSize = ImGui::CalcTextSize(text);
        float offsetX = (menuSizeX - textSize.x) * 0.5f;
        ImGui::SetCursorPosX(offsetX);
        ImGui::Text("%s", text);
        ImGui::Dummy(ImVec2(0.0f, buttonPosX * 0.6));

        ImGui::Indent(buttonPosX);

        if (ImGui::Button("Resume", ImVec2(buttonWidth, buttonHeight))) // -1 = whole width
        {
            exitPause();
        }

        if (ImGui::Button("Restart", ImVec2(buttonWidth, buttonHeight)))
        {
            restartGame();
        }

        if (ImGui::Button("Quit", ImVec2(buttonWidth, buttonHeight)))
        {
            glfwSetWindowShouldClose(window, true);
        }

        ImGui::PopFont();
        ImGui::Unindent(buttonPosX);

        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void Application::renderFrame()
{
    gameManager.shader->Use(); // Bind shader program

    setMatrices();              // Send model/view/projection matrices to shader
    setCameraUniforms();        // Send camera position to shader for lighting calculations
    setLightUniforms();         // Send lighting parameters to shader
    bindTexture();              // Bind texture and tell shader to use it
    configureCulling();         // Enable face culling (to improve performance)

    gameManager.scene.draw(gameManager.shader);


    drawUI();
}

void Application::updatePause()
{
    bool escapePressed = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;

    if (escapePressed && !escapePressedLastFrame)
    {
        isPaused = !isPaused;
    }

    escapePressedLastFrame = escapePressed;

    if (isPaused) 
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else 
    {
        glfwSetCursorPos(window, windowWidth / 2.0, windowHeight / 2.0);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void Application::exitPause()
{
    isPaused = false;
    glfwSetCursorPos(window, windowWidth / 2.0, windowHeight / 2.0);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Application::restartGame()
{
}


void Application::setMatrices()
{
    // Calculate the view, projection, and model matrices
    LibMath::Matrix4 view = gameManager.player.getCamera().GetViewMatrix();
    LibMath::Matrix4 projection = gameManager.player.getCamera().GetProjectionMatrix((float)windowWidth / windowHeight);
    LibMath::Matrix4 model = LibMath::Matrix4::getIdentity(); // Identity for root
    
    // Upload matrices to shader uniforms
    gameManager.shader->setMat4("view", view);
    gameManager.shader->setMat4("projection", projection);
    gameManager.shader->setMat4("model", model);
}

void Application::bindTexture()
{
    glUniform1i(glGetUniformLocation(gameManager.shader->GetID(), "useTexture"), false); // Tell shader to use texture
    glActiveTexture(GL_TEXTURE0);      // Activate texture unit 0
    glBindTexture(GL_TEXTURE_2D, texture1); // Bind texture to unit 0
}

void Application::configureCulling()
{
    glEnable(GL_CULL_FACE);  // Enable face culling
    glCullFace(GL_BACK);     // Cull back faces (do not draw polygons facing away)
    glFrontFace(GL_CCW);     // Define front face as counter-clockwise winding order
}

void Application::mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app || app->isPaused) return;

    float centerX = app->windowWidth / 2.0f;
    float centerY = app->windowHeight / 2.0f;

    float xoffset = xpos - centerX;
    float yoffset = centerY - ypos; // inversé pour l’axe Y

    app->gameManager.processMouseMovement(xoffset, yoffset);

    // Recentrer la souris
    glfwSetCursorPos(window, centerX, centerY);
}

void Application::setLightUniforms()
{
    GLuint shaderID = gameManager.shader->GetID();

    // Pass all point light parameters to shader uniforms
    glUniform3fv(glGetUniformLocation(shaderID, "pointLight.position"), 1, &pointLight.position.m_x);
    glUniform3fv(glGetUniformLocation(shaderID, "pointLight.base.ambient"), 1, &pointLight.ambient.m_x);
    glUniform3fv(glGetUniformLocation(shaderID, "pointLight.base.diffuse"), 1, &pointLight.diffuse.m_x);
    glUniform3fv(glGetUniformLocation(shaderID, "pointLight.base.specular"), 1, &pointLight.specular.m_x);
    glUniform1f(glGetUniformLocation(shaderID, "pointLight.constant"), pointLight.constant);
    glUniform1f(glGetUniformLocation(shaderID, "pointLight.linear"), pointLight.linear);
    glUniform1f(glGetUniformLocation(shaderID, "pointLight.quadratic"), pointLight.quadratic);
}

void Application::setCameraUniforms()
{
    LibMath::Vector3 camPos = gameManager.player.getCamera().position;

    GLint viewPosLoc = glGetUniformLocation(gameManager.shader->GetID(), "viewPos");

    // Pass camera position to shader for specular lighting calculations
    glUniform3fv(viewPosLoc, 1, &camPos.m_x);
}

unsigned int Application::loadTexture(const char* path)
{
    unsigned int textureID;

    glGenTextures(1, &textureID); // Generate OpenGL texture ID

    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0); // Load image file

    if (data)
    {
        // Decide texture format based on number of color channels
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

        glBindTexture(GL_TEXTURE_2D, textureID);
        // Upload texture data to GPU
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // Generate mipmaps for better scaling

        // Set texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Set filtering parameters (mipmap for minification, linear for magnification)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Failed to load texture: " << path << std::endl;
    }

    stbi_image_free(data); // Free image memory

    return textureID;
}

bool Application::setupResources()
{
    setupLighting();  // Initialize lighting parameters
    setupOpenGLStates();
    bindTexture();         // Load and bind texture for rendering

    return true;
}


void Application::setupLighting()
{
    // Initialize point light position and color properties (ambient, diffuse, specular)
    pointLight.position = LibMath::Vector3(.0f, .0f, .0f);
    pointLight.ambient = LibMath::Vector4(0.3f, 0.3f, 0.3f, 1.0f);
    pointLight.diffuse = LibMath::Vector4(0.9f, 0.9f, 0.9f, 1.0f);
    pointLight.specular = LibMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    pointLight.constant = 1.0f;
    pointLight.linear = 0.09f;
    pointLight.quadratic = 0.f;
}

void Application::setupOpenGLStates()
{
    glEnable(GL_DEPTH_TEST);  // Enable depth testing (hidden surface removal)
    glDepthFunc(GL_LESS);     // Accept fragment if closer than existing one
}
