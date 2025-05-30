#include "Application.h"
#include "GLTexture.h"
   

bool Application::init() {
    if (!initGLFW()) return false;
    if (!createWindow()) return false;
    if (!initGLAD()) return false;
    initGLContext();
    initImGuiContext(); // Setup imgui context, input modes 

    // Initialize scene and game objects
    gameManager.init();

    return true;
}

bool Application::initGLFW() {
    if (!glfwInit()) {
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

void Application::initGLContext()
{
    glfwMakeContextCurrent(window); 
    glfwSetWindowUserPointer(window, this); 

    glfwSwapInterval(0); 


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    // Register mouse movement callback function
    glfwSetCursorPosCallback(window, mouse_callback);
}

bool Application::createWindow() {
    window = glfwCreateWindow(1920, 1200, "3D Game", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    return true;
}

bool Application::initGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return false;
    }
    return true;
}

void Application::initGLContext() {
    // Set callbacks
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glEnable(GL_DEPTH_TEST);
}

void Application::run()
{
    while (!glfwWindowShouldClose(window)) // Main render loop until window is closed
    {
        updateDeltaTime(); 
        player.processInput(window, m_deltaTime);  
        clearScreen();    // Clear color and depth buffers

        renderFrame();    

        glfwSwapBuffers(window); // Present backbuffer to screen
        glfwPollEvents();        // Process window events 
    }
}

void Application::updateDeltaTime()
{
    float currentFrame = glfwGetTime(); 

    m_deltaTime = currentFrame - m_lastFrame; // Time difference between frames
    m_lastFrame = currentFrame;
}

void Application::clearScreen()
{
    // Set clear color to black and clear color and depth buffers
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Application::renderFrame()
{
    shaderProgram->Use(); // Bind shader program

    setMatrices();          // Send model/view/projection matrices to shader
    setCameraUniforms();    // Send camera position to shader for lighting calculations
    setLightUniforms();     // Send lighting parameters to shader
    bindTextureForRendering(); // Bind texture and tell shader to use it
    configureCulling();     // Enable face culling (to improve performance)


    updateSceneGraph();     
    drawSceneGraph();       // Draw the whole scene graph recursively
    drawUI();
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
    LibMath::Matrix4 view = player.getCamera().GetViewMatrix();
    LibMath::Matrix4 projection = player.getCamera().GetProjectionMatrix((float)windowWidth / windowHeight);
    LibMath::Matrix4 model = LibMath::Matrix4::getIdentity(); // Identity for root

    // Upload matrices to shader uniforms
    shaderProgram -> setMat4("view", view);
    shaderProgram -> setMat4("projection", projection);
    shaderProgram -> setMat4("model", model);
}

void Application::bindTextureForRendering()
{
    glUniform1i(glGetUniformLocation(shaderProgram -> GetID(), "useTexture"), true); // Tell shader to use texture
    glActiveTexture(GL_TEXTURE0);      // Activate texture unit 0
    glBindTexture(GL_TEXTURE_2D, texture1); // Bind texture to unit 0
}

void Application::configureCulling()
{
    glEnable(GL_CULL_FACE);  // Enable face culling
    glCullFace(GL_BACK);     // Cull back faces (do not draw polygons facing away)
    glFrontFace(GL_CCW);     // Define front face as counter-clockwise winding order
}

void Application::updateSceneGraph()
{
    float angle = glfwGetTime(); 

    rootNode->localTransform = LibMath::Matrix4::createTransform(LibMath::Vector3(0,0,0), LibMath::Vector3(0, 1, 0), LibMath::Radian(angle),LibMath::Vector3(1, 1, 1));

        // Update game logic
        gameManager.update(deltaTime);

        // Render
        clearScreen();
        gameManager.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
}

void Application::setCameraUniforms()
{
    LibMath::Vector3 camPos = player.getCamera().position;

    GLint viewPosLoc = glGetUniformLocation(shaderProgram->GetID(), "viewPos");

void Application::updateDeltaTime() {
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
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

    if (!loadShaders()) // Load vertex and fragment shaders
    {
        return false;
    }

    if (!loadModel())   // Load 3D model from file
    {
        return false;
    }

    buildSceneGraph();  
    model3D.uploadToGPU(); // Upload mesh data (vertices, indices) to GPU buffers
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

bool Application::loadShaders()
{
    // Load vertex and fragment shaders from files and link them into a program
    if (!shaderProgram -> SetVertexShader("../../Assets/Shaders/vertex.glsl")) return false;
    if (!shaderProgram -> SetFragmentShader("../../Assets/Shaders/fragment.glsl")) return false;
    return shaderProgram -> Link();
}

bool Application::loadModel()
{
    if (!model3D.load("../../Assets/Meshes/bunny.obj"))
    {
        std::cerr << "Failed to load model\n";

        return false;
    }
    return true;
}

void Application::buildSceneGraph()
{
    // Create two separate shader programs for root and child nodes
    Shader* shader1 = new Shader();
    shader1 -> SetVertexShader("../../Assets/Shaders/vertex.glsl");
    shader1 -> SetFragmentShader("../../Assets/Shaders/fragment.glsl");
    shader1 -> Link();

    Shader* shader2 = new Shader();
    shader2->SetVertexShader("../../Assets/Shaders/vertex.glsl");
    shader2->SetFragmentShader("../../Assets/Shaders/fragment.glsl");
    shader2->Link();

    // Create root and child scene graph nodes with models and shaders
    rootNode = std::make_shared<SceneNode<Model>>(&model3D, shader1);
    childNode = std::make_shared<SceneNode<Model>>(&model3D, shader2);

    rootNode -> addChild(childNode); // Attach child node to root node

    rootNode -> localTransform = LibMath::Matrix4::createTransform(LibMath::Vector3(0, 0, 0), LibMath::Vector3(0, 1, 0), LibMath::Radian(0), LibMath::Vector3(1, 1, 1));

    childNode->localTransform = LibMath::Matrix4::createTransform(LibMath::Vector3(5, 0, 0), LibMath::Vector3(0, 1, 0), LibMath::Radian(0), LibMath::Vector3(0.5, 0.5, 0.5));


    uiManager.addElement(std::make_unique<CircleUI>(
        LibMath::Vector2(200.0f, 300.0f), 30.0f, ImColor(255, 0, 0, 255)
    ));

}

void Application::setupOpenGLStates()
{
    glEnable(GL_DEPTH_TEST);  // Enable depth testing (hidden surface removal)
    glDepthFunc(GL_LESS);     // Accept fragment if closer than existing one
}

void Application::clearScreen() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Application::mouse_callback(GLFWwindow* window, double xpos, double ypos) 
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app) return;

    if (app->firstMouse) 
    {
        // Première fois : initialise lastX et lastY
        app->lastX = (float)xpos;
        app->lastY = (float)ypos;
        app->firstMouse = false;
        return;   // pas de mouvement à traiter
    }

    // Calcul des offsets
    float xoffset = (float)xpos - app->lastX;
    float yoffset = app->lastY - (float)ypos; // inversé : y va de bas en haut

    // Met à jour pour le prochain appel
    app->lastX = (float)xpos;
    app->lastY = (float)ypos;

    app -> player.getCamera().ProcessMouseMovement(
        xoffset * app -> player.getCamera().mouseSensitivity,
        yoffset * app -> player.getCamera().mouseSensitivity
    );
}