#LibGL

add_subdirectory(Dependencies)
add_subdirectory(Resources)
add_subdirectory(LowRenderer)


# Create an interface library to aggregate all sub-libraries
add_library(LibGL INTERFACE)

# Optional: For Visual Studio folders
set_target_properties(LibGL PROPERTIES FOLDER "LibGL")

# Include directories for consumers of ModernOpenGL (interface)
target_include_directories(LibGL INTERFACE
	${LOW_RENDERER_INCLUDE_DIR}
	${RESOURCES_INCLUDE_DIR}
	${DEPENDENCIES_INCLUDE_DIR}
)

# Link all sub-libraries to the interface ModernOpenGL target
target_link_libraries(LibGL INTERFACE
	${LOW_RENDERER}
	${RESOURCES_NAME}  
	${DEPENDENCIES_NAME}
)


# Export ModernOpenGL target to parent scope
set(LIBGL LibGL PARENT_SCOPE)
