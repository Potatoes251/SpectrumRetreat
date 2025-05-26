# Core/CMakeLists.txt

set(CORE_NAME Core)

# Collect code files
file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Header/*.h)
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp)

# Collect non-code files (shaders, assets)
file(GLOB_RECURSE SHADER_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/*.glsl)
file(GLOB_RECURSE ASSET_FILES CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets/**/*.obj
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets/**/*.tga
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets/**/*.jpg
    ${CMAKE_CURRENT_SOURCE_DIR}/Assets/**/*.png
)

# Group files for IDE (VS or others)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES 
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
    ${SHADER_FILES}
    ${ASSET_FILES}
)

# Create a library target instead of an executable
add_library(${CORE_NAME})

# Add source files
target_sources(${CORE_NAME} PRIVATE
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
)

# Add shaders/assets as header-only (visible in IDE, not compiled)
foreach(file IN LISTS SHADER_FILES ASSET_FILES)
    set_source_files_properties(${file} PROPERTIES HEADER_FILE_ONLY TRUE)
    target_sources(${CORE_NAME} PRIVATE ${file})
endforeach()

# Include directories
target_include_directories(${CORE_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Header
    PRIVATE ${LIB_INCLUDE_DIR}
    PRIVATE ${DATA_INCLUDE_DIR}
    PRIVATE ${LOW_INCLUDE_DIR}
    PRIVATE ${RESOURCES_INCLUDE_DIR}
)

# Link dependencies
target_link_libraries(${CORE_NAME}
    PRIVATE Dependencies
    PRIVATE opengl32
    PRIVATE user32 
    PRIVATE gdi32 
    PRIVATE shell32
    PRIVATE ${LIB_NAME}
    PRIVATE ${DATA_NAME}
    PRIVATE ${LOW_NAME}
    PRIVATE ${RESOURCES_NAME}
)

# Export the name and include dir to the parent scope
set(CORE_NAME ${CORE_NAME} PARENT_SCOPE)
set(CORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Header PARENT_SCOPE)