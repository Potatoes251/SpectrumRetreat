#include "Application.h"
#include "GLTexture.h"
#include "Player.h"


Application::~Application()
{
    // 1. Destroy ImGui resources (order matters)
    ImGui_ImplOpenGL3_DestroyDeviceObjects();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    // Optional: Clear font atlas data
    ImFontAtlas* fontAtlas = ImGui::GetIO().Fonts;
    fontAtlas -> Clear();
    fontAtlas -> ClearInputData();
    fontAtlas -> ClearTexData();

    // 2. Destroy the ImGui context
    ImGui::DestroyContext();

    // 3. Only now terminate GLFW
    glfwTerminate();
}

bool    Application::init()
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
    m_gameManager.init(m_windowWidth, m_windowHeight);
    m_gameManager.setApplication(this);

    if (!initShaderAndResources())
        return false;

    return true;
}

bool    Application::initGLFW() 
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

void    Application::initImGuiContext()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_io = &ImGui::GetIO(); (void)m_io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    m_gameManager.uiManager.fontSmall = m_io->Fonts->AddFontFromFileTTF("../../Assets/Fonts/Play-Regular.ttf", 16.0f);
    m_gameManager.uiManager.fontMedium = m_io->Fonts->AddFontFromFileTTF("../../Assets/Fonts/Play-Regular.ttf", 22.0f);
    m_gameManager.uiManager.fontLarge = m_io->Fonts->AddFontFromFileTTF("../../Assets/Fonts/Play-Regular.ttf", 32.0f);
}

bool    Application::createWindow() 
{
    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(primary);

    m_windowWidth = 1920;
    m_windowHeight = 1080;

    m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "3D Game", nullptr, nullptr);

    if (!m_window) 
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);

    return true;
}

bool    Application::initGLAD() 
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
    {
        std::cerr << "Failed to initialize GLAD\n";

        return false;
    }

    return true;
}

void    Application::initGLContext() 
{
    //Set callbacks
    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);

    glfwSwapInterval(0);

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    // Register mouse movement callback function
    glfwSetCursorPosCallback(m_window, mouse_callback);
}

bool    Application::initShaderAndResources()
{
    if (!setupResources()) // Load shaders, model, textures, set lighting and OpenGL states
    {
        std::cerr << "Failed to set up resources!\n";

        return false;
    }
    
    return true;
}

void    Application::run() 
{
    updateDeltaTime();
    while (!glfwWindowShouldClose(m_window)) 
    {
        updatePause();
        updateDeltaTime();

        if (!m_isPaused)
        {
            // update the velocity of the player
            m_gameManager.player.processInput(m_window, m_deltaTime, m_gameManager.scene, *m_gameManager.phonePtr, *m_gameManager.crosshairPtr);
            m_gameManager.player.update(m_deltaTime);

            // check the collisions
            bool restart;
            checkWalls(m_gameManager.player, m_gameManager.scene, m_gameManager.phonePtr->getColor(), restart);

            // apply velocity
            m_gameManager.player.move(m_deltaTime);

            // Update game logic
            m_gameManager.update(m_deltaTime);
            if (restart)
            {
                // restart if the endZone has been reached
                restartGame();
            }
        }

        // Render the scene
        clearScreen();
        renderFrame();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void    Application::updateDeltaTime() 
{
    float currentFrame = static_cast<float>(glfwGetTime());

    if (!m_isPaused)
    {
        m_deltaTime = currentFrame - m_lastFrame;
    }
    else
    {
        m_deltaTime = 0.0f;
    }

    m_lastFrame = currentFrame;
    m_gameManager.totalTime += m_deltaTime;
}

void    Application::clearScreen() 
{
    glClearColor(0.35f, 0.32f, 0.42f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void    Application::drawUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    m_gameManager.uiManager.drawAll();

    if (m_isPaused) // screen darkening in pause
    {
        m_gameManager.quitButton -> m_isActive      = true;
        m_gameManager.restartButton -> m_isActive   = true;
        m_gameManager.resumeButton -> m_isActive    = true;
        m_gameManager.crosshairPtr -> m_isActive    = false;
        m_gameManager.phonePtr -> m_isActive        = false;
    }
    else
    {
        m_gameManager.quitButton -> m_isActive      = false;
        m_gameManager.restartButton -> m_isActive   = false;
        m_gameManager.resumeButton -> m_isActive    = false;
        m_gameManager.crosshairPtr -> m_isActive    = true;
        m_gameManager.phonePtr -> m_isActive        = true;
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void    Application::renderFrame()
{
    m_gameManager.shader -> Use();  // Bind shader program

    setMatrices();                  // Send model/view/projection matrices to shader
    setCameraUniforms();            // Send camera position to shader for lighting calculations
    setLightUniforms();             // Send lighting parameters to shader
    bindTexture();                  // Bind texture and tell shader to use it
    configureCulling();             // Enable face culling (to improve performance)

    m_gameManager.scene.draw(m_gameManager.shader);


    drawUI();
}

void    Application::updatePause()
{
    bool escapePressed = glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS;

    if (escapePressed && !m_escapePressedLastFrame)
    {
        m_isPaused = !m_isPaused;
    }

    m_escapePressedLastFrame = escapePressed;

    if (m_isPaused) 
    {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else 
    {
        glfwSetCursorPos(m_window, m_windowWidth / 2.0, m_windowHeight / 2.0);
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void    Application::exitPause()
{
    m_isPaused = false;
    glfwSetCursorPos(m_window, m_windowWidth / 2.0, m_windowHeight / 2.0);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void    Application::restartGame()
{
    m_gameManager.restart(m_windowWidth, m_windowHeight);
    exitPause();
}

void Application::quitGame()
{
    glfwSetWindowShouldClose(getWindow(), true);
}


void    Application::setMatrices()
{
    // Calculate the view, projection, and model matrices
    LibMath::Matrix4 view = m_gameManager.player.getCamera().GetViewMatrix();
    LibMath::Matrix4 projection = m_gameManager.player.getCamera().GetProjectionMatrix((float)m_windowWidth / m_windowHeight);
    // Identity for root
    LibMath::Matrix4 model = LibMath::Matrix4::getIdentity(); 
    
    // Upload matrices to shader uniforms
    m_gameManager.shader->setMat4("view", view);
    m_gameManager.shader->setMat4("projection", projection);
    m_gameManager.shader->setMat4("model", model);
}

void    Application::bindTexture()
{
    // Tell shader to use texture
    glUniform1i(glGetUniformLocation(m_gameManager.shader->GetID(), "useTexture"), false); 
    glActiveTexture(GL_TEXTURE0);      // Activate texture unit 0
    glBindTexture(GL_TEXTURE_2D, m_texture1); // Bind texture to unit 0
}

void    Application::configureCulling()
{
    glEnable(GL_CULL_FACE);  // Enable face culling
    glCullFace(GL_BACK);     // Cull back faces (do not draw polygons facing away)
    glFrontFace(GL_CCW);     // Define front face as counter-clockwise winding order
}

void    Application::mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app || app->m_isPaused) return;

    float centerX = app->m_windowWidth / 2.0f;
    float centerY = app->m_windowHeight / 2.0f;

    float xoffset = xpos - centerX;
    // inversed for Y axis
    float yoffset = centerY - ypos;

    app -> m_gameManager.processMouseMovement(xoffset, yoffset);

    // Recentrer la souris
    glfwSetCursorPos(window, centerX, centerY);
}

void    Application::setLightUniforms()
{
    GLuint shaderID = m_gameManager.shader -> GetID();

    // Pass all point light parameters to shader uniforms
    for (size_t i = 0; i < m_gameManager.pointLights.size(); ++i)
    {
        const PointLight& light = m_gameManager.pointLights[i];
        std::string baseName = "pointLights[" + std::to_string(i) + "]";

        glUniform3fv(glGetUniformLocation(shaderID, (baseName + ".position").c_str()),      1,  &light.position.m_x);
        glUniform3fv(glGetUniformLocation(shaderID, (baseName + ".base.ambient").c_str()),  1,  &light.ambient.m_x);
        glUniform3fv(glGetUniformLocation(shaderID, (baseName + ".base.diffuse").c_str()),  1,  &light.diffuse.m_x);
        glUniform3fv(glGetUniformLocation(shaderID, (baseName + ".base.specular").c_str()), 1,  &light.specular.m_x);
        glUniform1f(glGetUniformLocation( shaderID, (baseName + ".constant").c_str()),          light.constant);
        glUniform1f(glGetUniformLocation( shaderID, (baseName + ".linear").c_str()),            light.linear);
        glUniform1f(glGetUniformLocation( shaderID, (baseName + ".quadratic").c_str()),         light.quadratic);
    }

    glUniform1i(glGetUniformLocation(shaderID, "numPointLights"), m_gameManager.pointLights.size());

    for (size_t i = 0; i < m_gameManager.spotLights.size(); ++i)
    {
        const SpotLight& light = m_gameManager.spotLights[i];
        std::string baseName = "spotLights[" + std::to_string(i) + "]";

        glUniform3fv(glGetUniformLocation(shaderID, (baseName + ".position").c_str()),      1,  &light.position.m_x);
        glUniform3fv(glGetUniformLocation(shaderID, (baseName + ".direction").c_str()),     1,  &light.direction.m_x);
        glUniform3fv(glGetUniformLocation(shaderID, (baseName + ".base.ambient").c_str()),  1,  &light.ambient.m_x);
        glUniform3fv(glGetUniformLocation(shaderID, (baseName + ".base.diffuse").c_str()),  1,  &light.diffuse.m_x);
        glUniform3fv(glGetUniformLocation(shaderID, (baseName + ".base.specular").c_str()), 1,  &light.specular.m_x);
        glUniform1f(glGetUniformLocation( shaderID, (baseName + ".constant").c_str()),          light.constant);
        glUniform1f(glGetUniformLocation( shaderID, (baseName + ".linear").c_str()),            light.linear);
        glUniform1f(glGetUniformLocation( shaderID, (baseName + ".quadratic").c_str()),         light.quadratic);
        glUniform1f(glGetUniformLocation( shaderID, (baseName + ".cutOff").c_str()),            light.cutOff);
        glUniform1f(glGetUniformLocation( shaderID, (baseName + ".outerCutOff").c_str()),       light.outerCutOff);
    }

    glUniform1i(glGetUniformLocation(shaderID, "numSpotLights"), m_gameManager.spotLights.size());
}

void    Application::setCameraUniforms()
{
    LibMath::Vector3 camPos = m_gameManager.player.getCamera().position;

    GLint viewPosLoc = glGetUniformLocation(m_gameManager.shader -> GetID(), "viewPos");

    // Pass camera position to shader for specular lighting calculations
    glUniform3fv(viewPosLoc, 1, &camPos.m_x);
}

unsigned int    Application::loadTexture(const char* path)
{
    unsigned int textureID;

    // Generate OpenGL texture ID
    glGenTextures(1, &textureID); 

    int width, height, nrChannels;

    // Load image file
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0); 

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

    // Free image memory
    stbi_image_free(data); 

    return textureID;
}

bool    Application::setupResources()
{
    // Initialize lighting parameters
    setupLighting();  
    setupOpenGLStates();
    // Load and bind texture for rendering
    bindTexture();         

    return true;
}


void    Application::setupLighting()
{
    PointLight p;
    p.position = LibMath::Vector3(1.0f, 1.0f, 1.0f );
    m_gameManager.pointLights.push_back(p);
    PointLight p2;
    p2.position = LibMath::Vector3(1.0f, 5.0f, -20.0f);
    m_gameManager.pointLights.push_back(p2);
    PointLight p3;
    p3.position = LibMath::Vector3(1.0f, 5.0f, -40.0f);
    m_gameManager.pointLights.push_back(p3);
    //PointLight p4;
    //p4.position = LibMath::Vector3(1.0f, 5.0f, -60.0f);
    //m_gameManager.pointLights.push_back(p4);
    //PointLight p5;
    //p5.position = LibMath::Vector3(1.0f, 5.0f, 1.0f);
    //m_gameManager.pointLights.push_back(p5);
    //PointLight p6;
    //p6.position = LibMath::Vector3(1.0f, 5.0f, -80.0f);
    //m_gameManager.pointLights.push_back(p6);
    //PointLight p7;
    //p7.position = LibMath::Vector3(1.0f, 5.0f, -100.0f);
    //m_gameManager.pointLights.push_back(p7);
    //PointLight p8;
    //p8.position = LibMath::Vector3(1.0f, 5.0f, -120.0f);
    //m_gameManager.pointLights.push_back(p8);

    // Initialize point light position and color properties (ambient, diffuse, specular)
    for (size_t i = 0; i < m_gameManager.pointLights.size(); ++i)
    {
        m_gameManager.pointLights[i].ambient = LibMath::Vector4(0.15f, 0.15f, 0.15f, 1.0f);
        m_gameManager.pointLights[i].diffuse = LibMath::Vector4(0.35f, 0.35f, 0.35f, 1.0f);
        m_gameManager.pointLights[i].specular = LibMath::Vector4(0.1f, 0.1f, 0.1f, 0.0f);
        m_gameManager.pointLights[i].constant = 1.0f;
        m_gameManager.pointLights[i].linear = 0.09f;
        m_gameManager.pointLights[i].quadratic = 0.f;
    }

    SpotLight p9;
    p9.position = LibMath::Vector3(1.0f, 5.0f, -80.0f);
    p9.direction = LibMath::Vector3(0.0f, -1.0f, 0.0f);

    float innerAngleDeg = 1;
    float outerAngleDeg = 3;

    p9.cutOff = cos(LibMath::Radian(innerAngleDeg).raw());
    p9.outerCutOff = cos(LibMath::Radian(outerAngleDeg).raw());

    m_gameManager.spotLights.push_back(p9);

    // Initialize spot light position and color properties (ambient, diffuse, specular)
    for (size_t i = 0; i < m_gameManager.spotLights.size(); ++i)
    {
        m_gameManager.spotLights[i].ambient = LibMath::Vector4(0.15f, 0.15f, 0.15f, 1.0f);
        m_gameManager.spotLights[i].diffuse = LibMath::Vector4(0.35f, 0.35f, 0.35f, 1.0f);
        m_gameManager.spotLights[i].specular = LibMath::Vector4(0.1f, 0.1f, 0.1f, 0.0f);
        m_gameManager.spotLights[i].constant = 1.0f;
        m_gameManager.spotLights[i].linear = 0.09f;
        m_gameManager.spotLights[i].quadratic = 0.f;
    }
}

void Application::setupOpenGLStates()
{
    // Enable depth testing (hidden surface removal)
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if closer than existing one
    glDepthFunc(GL_LESS);     
}
