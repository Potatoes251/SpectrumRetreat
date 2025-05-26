# ModernOpenGL/CMakeLists.txt

# Add all subdirectories (each builds its own static/shared lib)
add_subdirectory(DataStructure)
add_subdirectory(Dependencies)
add_subdirectory(LowRenderer)
add_subdirectory(Resources)
add_subdirectory(Core)

# Create an interface library to aggregate all sub-libraries
add_library(ModernOpenGL INTERFACE)

# Optional: For Visual Studio folders
set_target_properties(ModernOpenGL PROPERTIES FOLDER "ModernOpenGL")

# Link all sub-libraries to the interface ModernOpenGL target
target_link_libraries(ModernOpenGL INTERFACE
    ${CORE_NAME}
    ${DATASTRUCTURE_NAME}
    ${DEPENDENCIES_NAME}
    ${LOWRENDERER_NAME}
    ${RESOURCES_NAME}
)

# Include directories for consumers of ModernOpenGL (interface)
target_include_directories(ModernOpenGL INTERFACE
    ${CORE_INCLUDE_DIR}
    ${DATASTRUCTURE_INCLUDE_DIR}
    ${DEPENDENCIES_INCLUDE_DIR}
    ${LOWRENDERER_INCLUDE_DIR}
    ${RESOURCES_INCLUDE_DIR}
)

# Export ModernOpenGL target to parent scope
set(MODERN_OPENGL ModernOpenGL PARENT_SCOPE)