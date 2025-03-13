cmake_minimum_required(VERSION 3.29)
project(voxel_lab)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Add dependencies
add_subdirectory(external/glfw)
add_subdirectory(external/glm)
add_subdirectory(external/imgui)

# GLAD configuration
add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# Main executable with all sources
add_executable(${PROJECT_NAME}
    main.cpp
    core/Application.cpp
    core/InputHandler.cpp
    graphics/Shader.cpp
    graphics/Texture.cpp
    graphics/Renderer.cpp
    terrain/Terrain.cpp
    terrain/Chunk.cpp
    graphics/stb_image.c  # Add STB implementation
    #    ui/DebugUI.cpp
    util/Tick.cpp
    util/FrameTimer.cpp
)

# Compiler definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    glad
    glfw
    imgui
    OpenGL::GL
    # Add system libraries if needed
    ${CMAKE_DL_LIBS}
    pthread
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    core/
    graphics/
    terrain/
    ui/
    util/
    external/imgui
    external/glad/include
    external/stb  # If stb_image.h is in a subdirectory
)
