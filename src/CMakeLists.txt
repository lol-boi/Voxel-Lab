cmake_minimum_required(VERSION 3.29)
project(voxel_lab)

# Add GLFW3
#find_package(glfw3 3.4 REQUIRED)
add_subdirectory(libs/glfw)

# Add OpenGL
find_package(OpenGL REQUIRED)

# Add GLAD library
add_library(glad SHARED libs/glad/src/glad.c)
target_include_directories(glad PUBLIC libs/glad/include)

#Add Imgui library
add_library(imgui SHARED libs/imgui/imgui.cpp libs/imgui/imgui_impl_glfw.cpp libs/imgui/imgui_impl_opengl3.cpp libs/imgui/imgui_draw.cpp libs/imgui/imgui_widgets.cpp libs/imgui/imgui_tables.cpp libs/imgui/imgui_demo.cpp)
target_include_directories(imgui PUBLIC libs/imgui)
target_link_libraries(imgui PUBLIC OpenGL::GL glfw)

# Add stb_image library
add_library(image SHARED libs/stb_image.c)
target_include_directories(image PUBLIC libs)

# Add shader library
add_library(shader SHARED libs/shader.cpp)
target_include_directories(shader PUBLIC libs)
target_link_libraries(shader glad)

# Add chunk library
add_library(chunk SHARED libs/Chunk.cpp)
target_include_directories(chunk PUBLIC libs)
target_link_libraries(chunk glad)

# Add terrain library
add_library(terrain SHARED libs/Terrain.cpp)
target_include_directories(terrain PUBLIC libs)
target_link_libraries(terrain chunk glad)

# Add GLM library manually
add_library(glm INTERFACE)
set(GLM_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/libs/glm")
target_include_directories(glm INTERFACE ${GLM_INCLUDE_DIR})


# Add the executable
add_executable(${PROJECT_NAME} main.cpp)

# Link the libraries
target_link_libraries(${PROJECT_NAME} terrain shader image glad OpenGL::GL glfw glm imgui )
