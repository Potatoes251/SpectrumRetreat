#Mother
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(USE_FOLDERS ON)
project(ProjetFinAnnee)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(nom_du_groupe Ewan_Xavier_Mateo) #change the name 
add_subdirectory(Assets)
add_subdirectory(LibMath)
add_subdirectory(LibGL)
add_subdirectory(Platformer)


#        Dear ImGui Setup       #
#################################

# Définir le dossier où est ImGui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/External/imgui)
set(EXE_NAME Platformer)
# Fichiers source d'ImGui
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Include directories
target_include_directories(${EXE_NAME} PRIVATE ${IMGUI_DIR})
target_include_directories(${EXE_NAME} PRIVATE ${IMGUI_DIR}/backends)

# Ajouter les fichiers sources au projet
target_sources(${EXE_NAME} PRIVATE ${IMGUI_SOURCES})

if (MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Platformer)
endif()