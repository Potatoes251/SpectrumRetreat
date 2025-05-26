# Get folder name to use as executable name (Plateformer)
get_filename_component(EXE_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

# Create executable target
add_executable(${EXE_NAME})

# Collect all source and header files recursively
file(GLOB_RECURSE PROJECT_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
)

# Add sources to the executable
target_sources(${EXE_NAME} PRIVATE ${PROJECT_FILES})

# Include the public include directory of Plateformer
target_include_directories(${EXE_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link all ModernOpenGL libraries (make sure these variables are set in ModernOpenGL submodules)
target_link_libraries(${EXE_NAME} PRIVATE
    ${CORE_NAME}
    ${DATASTRUCTURE_NAME}
    ${DEPENDENCIES_NAME}
    ${LOWRENDERER_NAME}
    ${PHYSICS_NAME}
    ${RESOURCES_NAME}
)

# If you have other external libs to link, do it here
# e.g. target_link_libraries(${EXE_NAME} PRIVATE glfw3 opengl32)