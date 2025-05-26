project(Dependencies LANGUAGES C CXX)

# === Header Files ===
file(GLOB_RECURSE GLAD_HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Header/glad/*.h)
file(GLOB_RECURSE GLFW_HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Header/GLFW/*.h)
file(GLOB_RECURSE KHR_HEADERS  CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Header/KHR/*.h)
set(OTHER_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/Header/stb_image.h)

# === Source Files ===
set(SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/Source/glad.c)

# === Group for IDE ===
source_group("Header Files\\glad" FILES ${GLAD_HEADERS})
source_group("Header Files\\GLFW" FILES ${GLFW_HEADERS})
source_group("Header Files\\KHR"  FILES ${KHR_HEADERS})
source_group("Header Files"       FILES ${OTHER_HEADERS})
source_group("Source Files"       FILES ${SRC_FILES})

# === Create Dependencies lib ===
add_library(Dependencies STATIC
    ${GLAD_HEADERS} ${GLFW_HEADERS} ${KHR_HEADERS}
    ${OTHER_HEADERS}
    ${SRC_FILES}
)

target_include_directories(Dependencies PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Header)

# === glfw3.lib (prebuilt static) ===
set(GLFW_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib)

if(NOT EXISTS ${GLFW_LIB_PATH})
    message(FATAL_ERROR "Missing glfw3.lib at ${GLFW_LIB_PATH}")
endif()

add_library(glfw3 STATIC IMPORTED GLOBAL)
set_target_properties(glfw3 PROPERTIES
    IMPORTED_LOCATION ${GLFW_LIB_PATH}
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/Header
)

target_link_libraries(Dependencies PUBLIC glfw3)