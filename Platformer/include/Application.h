#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shader.h"
#include "Buffer.h"
#include "VertexAttributes.h"
#include "Camera.h"
#include "Model.h"
#include "light.h"
#include "SceneNode.h"
#include "../../Platformer/header/GameManager.h"

//class Application
//{
//public:
//    Application(int width, int height, const char* title)
//        : windowWidth(width), windowHeight(height), windowTitle(title), window(nullptr), shaderProgram(nullptr) {
//    }
//
//    ~Application()
//    {
//        if (shaderProgram) delete shaderProgram;
//        if (window) glfwDestroyWindow(window);
//        glfwTerminate();
//    }
//
//    bool init();
//    bool initGLFW();
//    void initGLContext();
//    bool createWindow();
//    bool initGLAD();
//    bool initShaderAndResources();
//    void run();
//    void updateDeltaTime();
//    void renderFrame();
//    void setMatrices();
//    void bindTextureForRendering();
//    void configureCulling();
//    void updateSceneGraph();
//    void drawSceneGraph();
//    void clearScreen();
//    void setLightUniforms();
//    void setCameraUniforms();
//    unsigned int loadTexture(const char* path);
//    bool setupResources();
//    void setupLighting();
//    bool loadShaders();
//    bool loadModel();
//    void buildSceneGraph();
//    void setupOpenGLStates();
//    void bindTexture();
//
//    float lastX = 800.0f / 2.0f;
//    float lastY = 600.0f / 2.0f;
//    bool firstMouse = true;
//    float yaw = -90.0f;
//    float pitch = 0.0f;
//
//    float sensitivity = 0.1f;
//
//    CameraFPS camera = CameraFPS(
//        LibMath::Vector3(0.0f, 0.0f, 3.0f),
//        LibMath::Vector3(0.0f, 1.0f, 0.0f)
//    );
//
//    GameManager gameManager;
//
//    Model model3D;
//    PointLight pointLight;
//    SpotLight spotLight;
//
//    ResourceManager resourceManager;
//
//    float m_rotationAngle = 0.0f;
//
//private:
//    void processInput();
//
//    int windowWidth;
//    int windowHeight;
//    const char* windowTitle;
//    GLFWwindow* window;
//
//    Shader* shaderProgram;
//
//    Buffer m_VBO;               // Vertex Buffer Object
//    Buffer m_EBO;               // Element Buffer Object
//    VertexAttributes m_VAO;     // Vertex Array Object
//
//    SceneNode<Model>::Ptr rootNode;
//    SceneNode<Model>::Ptr childNode;
//
//    unsigned int texture1;
//    unsigned int texture2;
//
//
//    float m_deltaTime = 0.0f;
//    float m_lastFrame = 0.0f;
//
//    
//};
//
//    void mouse_callback(GLFWwindow* window, double xpos, double ypos);


class Application {
public:
    bool init();
    void run();

private:
    GLFWwindow* window = nullptr;
    GameManager gameManager;

    // Timing
    float lastFrame = 0.0f;
    float deltaTime = 0.0f;

    bool firstMouse = true;
    float lastX = 0.0f, lastY = 0.0f;

    // Callbacks
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

    // Helpers
    bool initGLFW();
    bool createWindow();
    bool initGLAD();
    void initGLContext();
    void processInput();
    void updateDeltaTime();
    void clearScreen();
};