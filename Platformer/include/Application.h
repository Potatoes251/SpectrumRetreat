#ifndef APPLICATION_H
#define APPLICATION_H

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
    ~Application();

    bool    init();
    bool    setupResources();
    bool    initShaderAndResources();
    void    run();
    void    initImGuiContext();
    void    drawUI();
    void    setMatrices();
    void    bindTexture();
    void    configureCulling();
    void    setLightUniforms();
    void    setCameraUniforms();
    void    setupLighting();
    void    setupOpenGLStates();
    void    renderFrame();
    void    updatePause();
    void    exitPause();
    void    restartGame();
    void    quitGame();

    unsigned int    loadTexture(const char* path);

    GLFWwindow*     getWindow() const { return m_window; }

    GameManager     m_gameManager;

private:
    GLFWwindow*     m_window = nullptr;
    ImGuiIO*        m_io;
    
    bool    m_escapePressedLastFrame = false;
    bool    m_isPaused = false;

    // Timing
    float   m_lastFrame = 0.0f;
    float   m_deltaTime = 0.0f;

    bool    m_firstMouse = true;
    float   m_lastX = 0.0f, m_lastY = 0.0f;

    float   m_sensitivity = 0.1f;

    unsigned int    m_texture1;
    unsigned int    m_texture2;

    int     m_windowWidth;
    int     m_windowHeight;

    // Callbacks
    static void     mouse_callback(GLFWwindow* window, double xpos, double ypos);

    // Helpers
    bool    initGLFW();
    bool    createWindow();
    bool    initGLAD();
    void    initGLContext();
    void    processInput();
    void    updateDeltaTime();
    void    clearScreen();
};

#endif