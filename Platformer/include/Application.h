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
#include "UIManager.h"
#include "CircleUI.h"
#include "GameManager.h"

class Application
{
public:
    bool init();
    bool setupResources();
    bool initShaderAndResources();
    void run();
    void initImGuiContext();
    void drawUI();
    void setMatrices();
    void bindTexture();
    void configureCulling();
    void setLightUniforms();
    void setCameraUniforms();
    void setupLighting();
    void setupOpenGLStates();
    void renderFrame();
    void updatePause();
    void exitPause();
    void restartGame();
    unsigned int loadTexture(const char* path);

    GameManager gameManager;
private:
    GLFWwindow* window = nullptr;
    ImGuiIO* io;
    
    bool escapePressedLastFrame = false;
    bool isPaused = false;

    // Timing
    float lastFrame = 0.0f;
    float deltaTime = 0.0f;

    bool firstMouse = true;
    float lastX = 0.0f, lastY = 0.0f;

    float sensitivity = 0.1f;

    PointLight pointLight;
    SpotLight spotLight;

    float m_rotationAngle = 0.0f;

    unsigned int texture1;
    unsigned int texture2;

    int windowWidth;
    int windowHeight;

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