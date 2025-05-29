#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Application.h"

int main()
{
    Application app(2560, 1440, "wfgewaydshbjxnkm");

    if (!app.init())
    {
        std::cerr << "Failed to initialize application\n";
        return -1;
    }

    app.run();

    glfwTerminate();

    return 0;
}