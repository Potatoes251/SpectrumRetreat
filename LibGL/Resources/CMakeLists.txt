set(RESOURCES_NAME Resources) #Local variable only

file(GLOB_RECURSE PROJECT_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Header/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp
)

add_library(${RESOURCES_NAME})

target_sources(${RESOURCES_NAME} PRIVATE ${PROJECT_FILES})

target_include_directories(${RESOURCES_NAME} 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Header
    PRIVATE ${LIB_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(${RESOURCES_NAME}
    PRIVATE ${LIB_NAME}
    PRIVATE Dependencies
)

set(RESOURCES_NAME ${RESOURCES_NAME} PARENT_SCOPE) #outer scope variable only

set(RESOURCES_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Header PARENT_SCOPE)