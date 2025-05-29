#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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
#include "LibMath/Geometry3D.h"
#include "Player.h"

class Application
{
public:
    Application(int width, int height, const char* title)
        : windowWidth(width), windowHeight(height), windowTitle(title), window(nullptr), shaderProgram(nullptr) {
    }

    ~Application()
    {
        if (shaderProgram) delete shaderProgram;
        if (window) glfwDestroyWindow(window);
        glfwTerminate();
    }

    bool init();
    bool initGLFW();
    void initImGuiContext();
    void initGLContext();
    bool createWindow();
    bool initGLAD();
    bool initShaderAndResources();
    void run();
    void updateDeltaTime();
    void renderFrame();
    void setMatrices();
    void bindTextureForRendering();
    void configureCulling();
    void updateSceneGraph();
    void drawSceneGraph();
    void clearScreen();
    void setLightUniforms();
    void setCameraUniforms();
    unsigned int loadTexture(const char* path);
    bool setupResources();
    void setupLighting();
    bool loadShaders();
    bool loadModel();
    void buildSceneGraph();
    void setupOpenGLStates();
    void bindTexture();

    float lastX = 800.0f / 2.0f;
    float lastY = 600.0f / 2.0f;
    bool firstMouse = true;
    float yaw = -90.0f;
    float pitch = 0.0f;

    float sensitivity = 0.1f;

    Player player;
    Model model3D;
    PointLight pointLight;
    SpotLight spotLight;

    ResourceManager resourceManager;

    float m_rotationAngle = 0.0f;

private:

    int windowWidth;
    int windowHeight;
    const char* windowTitle;
    GLFWwindow* window;
    ImGuiIO* io;

    Shader* shaderProgram;

    Buffer m_VBO;               // Vertex Buffer Object
    Buffer m_EBO;               // Element Buffer Object
    VertexAttributes m_VAO;     // Vertex Array Object

    SceneNode<Model>::Ptr rootNode;
    SceneNode<Model>::Ptr childNode;

    unsigned int texture1;
    unsigned int texture2;


    float m_deltaTime = 0.0f;
    float m_lastFrame = 0.0f;

    
};

    void mouse_callback(GLFWwindow* window, double xpos, double ypos);