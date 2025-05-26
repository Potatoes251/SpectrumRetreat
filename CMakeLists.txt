cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(Xavier_Mateo_Ewan LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable nice folder grouping in IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add ModernOpenGL library (which adds its own submodules)
add_subdirectory(ModernOpenGL)

# Add the executable Plateformer
add_subdirectory(Plateformer)

# If you're on Windows with MSVC, set the startup project in IDE:
if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Plateformer)
endif()