set(DATA_NAME DataStructure) #Local variable only

file(GLOB_RECURSE PROJECT_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Header/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.cpp
)

add_library(${DATA_NAME})

target_sources(${DATA_NAME} PRIVATE ${PROJECT_FILES})

target_include_directories(${DATA_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Header
    PRIVATE ${LIB_INCLUDE_DIR}
    PRIVATE ${LOW_INCLUDE_DIR}
    PRIVATE ${PHYSICS_INCLUDE_DIR}
    PRIVATE ${RESOURCES_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(${DATA_NAME}
    PRIVATE ${LIB_NAME}
    PRIVATE ${LOW_NAME}
    PRIVATE ${PHYSICS_NAME}
    PRIVATE ${RESOURCES_NAME}
)


set(DATA_NAME ${DATA_NAME} PARENT_SCOPE) #outer scope variable only

set(DATA_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Header PARENT_SCOPE)