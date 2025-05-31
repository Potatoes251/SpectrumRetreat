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

    // Initialize scene and game objects
    gameManager.init();

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
    //gameManager.shader = new Shader();

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
        updateDeltaTime();
        gameManager.player.processInput(window, deltaTime);

        // Update game logic
        updateSceneGraph();
        gameManager.update(deltaTime);

        // Render
        clearScreen();
        gameManager.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}

void Application::updateDeltaTime() 
{
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}

void Application::updateSceneGraph()
{
    float angle = glfwGetTime();

    auto node = gameManager.scene.findNodeByName("Lapinou");
    
    if (node) 
    {
        //node->localTransform =
        //    LibMath::Matrix4::createTranslation({ 0,10,0 }) *
        //    LibMath::Matrix4::createRotationY(angle) *
        //    LibMath::Matrix4::createScale({ 0.1f,0.1f,0.1f });
    }

    gameManager.scene.root->updateTransform();
}

void Application::processInput() 
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    // ... camera or player input could go here or inside GameManager
}

void Application::clearScreen() 
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Application::drawUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    uiManager.drawAll(); // Draws all elements

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
    glUniform1i(glGetUniformLocation(gameManager.shader->GetID(), "useTexture"), true); // Tell shader to use texture
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
    if (!app) return;

    if (app->firstMouse) 
    {
        app->lastX = (float)xpos;
        app->lastY = (float)ypos;
        app->firstMouse = false;

        return;
    }

    float xoffset = (float)xpos - app->lastX;
    float yoffset = app->lastY - (float)ypos;

    app->lastX = (float)xpos;
    app->lastY = (float)ypos;

    app->gameManager.processMouseMovement(xoffset, yoffset);
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

    //if (!loadShaders()) // Load vertex and fragment shaders
    //{
    //    return false;
    //}

    //if (!loadModel())   // Load 3D model from file
    //{
    //    return false;
    //}

    //buildSceneGraph();
    //model3D.uploadToGPU(); // Upload mesh data (vertices, indices) to GPU buffers
    setupOpenGLStates();
    bindTexture();         // Load and bind texture for rendering

    return true;
}


void Application::setupLighting()
{
    // Initialize point light position and color properties (ambient, diffuse, specular)
    pointLight.position = LibMath::Vector3(2.0f, 2.0f, 2.0f);
    pointLight.ambient = LibMath::Vector4(0.3f, 0.3f, 0.3f, 1.0f);
    pointLight.diffuse = LibMath::Vector4(0.9f, 0.9f, 0.9f, 1.0f);
    pointLight.specular = LibMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    pointLight.constant = 1.0f;
    pointLight.linear = 0.09f;
    pointLight.quadratic = 0.032f;
}

void Application::setupOpenGLStates()
{
    glEnable(GL_DEPTH_TEST);  // Enable depth testing (hidden surface removal)
    glDepthFunc(GL_LESS);     // Accept fragment if closer than existing one
}
