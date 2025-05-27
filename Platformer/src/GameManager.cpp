#include "../header/GameManager.h"
#include <GLFW/glfw3.h>

GameManager::GameManager()
	: /*player(nullptr), scene(nullptr),*/ isRunning(true) {}

GameManager::~GameManager() {}

void GameManager::init()
{
	//player = new Player() = ;
}

void GameManager::run() 
{
    while (isRunning && !glfwWindowShouldClose(glfwGetCurrentContext())) 
    {
        processInput();
        update();
        render();
    }
}

void GameManager::processInput()
{
}

void GameManager::update()
{
    scene.update();
}

void GameManager::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene.draw();
    glfwSwapBuffers(glfwGetCurrentContext());
}
