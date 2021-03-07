cmake_minimum_required(VERSION 3.15)

project(GAMES102-HW10)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(GLFW_DIR ${LIB_DIR}/glfw)
set(GLAD_DIR ${LIB_DIR}/glad)
set(IMGUI_DIR ${LIB_DIR}/imgui)
set(SHADER_DIR ${LIB_DIR}/shader)
aux_source_directory(${SRC_DIR} SRC_LIST)
aux_source_directory(${IMGUI_DIR} IMGUI_LIST)
aux_source_directory(${SHADER_DIR} SHADER_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST} ${IMGUI_LIST} ${SHADER_LIST})

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR})

# glfw lib
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW documentation" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW tests" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW examples" FORCE)
add_subdirectory(${GLFW_DIR})
target_link_libraries(${PROJECT_NAME} glfw)

# glad lib
add_library(glad ${GLAD_DIR}/src/glad.c)
target_include_directories(glad PRIVATE ${GLAD_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLAD_DIR}/include)
target_link_libraries(${PROJECT_NAME} glad)
# target_link_libraries(${PROJECT_NAME} glad ${CMAKE_DL_LIBS})

# imgui lib
target_include_directories(${PROJECT_NAME} PRIVATE ${IMGUI_DIR})

# GLM
FetchContent_Declare(
    GLM
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG bf71a834948186f4097caa076cd2663c69a10e1e
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(GLM)
target_link_libraries(${PROJECT_NAME} glm)
#FetchContent_GetProperties(GLM)
#if (NOT GLM_POPULATED)
#    FetchContent_Populate(GLM)
#    add_subdirectory(${GLM_SOURCE_DIR} ${GLM_BINARY_DIR})
#endif()