#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Application.h"
#include "memoryLeak.h"

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);

    {
        Application app;
        
        if (!app.init())
        {
            std::cerr << "Failed to initialize application\n";
            return -1;
        }

        app.run();

        //char* test = new char;
    }

    _CrtDumpMemoryLeaks();

    return 0;
}