cmake_minimum_required(VERSION 3.29)
project(voxel_lab)

# Add GLFW3
#find_package(glfw3 3.4 REQUIRED)
add_subdirectory(external/glfw)

# Add OpenGL
find_package(OpenGL REQUIRED)

# Add GLAD library
add_library(glad SHARED external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# Add GLM library manually
add_subdirectory(external/glm)

# Add Imgui library
file(GLOB IMGUI_SOURCES external/imgui/*.cpp)
add_library(imgui SHARED ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC external/imgui)
target_link_libraries(imgui PUBLIC OpenGL::GL glfw)

# Set common include directory for all libraries
set(LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)

# Add libraries with consistent pattern
add_library(image SHARED ${LIBS_DIR}/stb_image.c)
add_library(shader SHARED ${LIBS_DIR}/shader.cpp)
add_library(chunk SHARED ${LIBS_DIR}/Chunk.cpp)
add_library(terrain SHARED ${LIBS_DIR}/Terrain.cpp)

# Set common include directory for all libraries
foreach(lib image shader chunk terrain)
    target_include_directories(${lib} PUBLIC ${LIBS_DIR})
endforeach()

# Set dependencies
target_link_libraries(shader PUBLIC glad)
target_link_libraries(chunk PUBLIC glad)
target_link_libraries(terrain PUBLIC chunk glad)

# Add the executable
add_executable(${PROJECT_NAME} main.cpp)

# Link the libraries
target_link_libraries(${PROJECT_NAME} terrain shader image glad OpenGL::GL glfw glm imgui )
