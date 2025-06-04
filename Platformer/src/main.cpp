#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <../../External/irrKlang/include/irrKlang.h>

#include "Application.h"

int main()
{
    Application app;

    irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

    if (!engine) 
    {
        std::cerr << "Erreur : impossible d'initialiser IrrKlang." << std::endl;
        return 1;
    }

    engine->play2D("../../Assets/Sounds/fragments of hope.ogg", true); // true = boucle

    if (!app.init())
    {
        std::cerr << "Failed to initialize application\n";
        return -1;
    }

    app.run();
    engine->drop();

    glfwTerminate();

    return 0;
}