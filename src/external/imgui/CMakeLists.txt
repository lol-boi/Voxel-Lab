cmake_minimum_required(VERSION 3.10...3.29)
project(imgui)

# Set source files (keep existing)
set(IMGUI_SOURCES
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui_impl_glfw.cpp
    imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/backends
)

# Update OpenGL target name
target_link_libraries(imgui PUBLIC
    glfw
    OpenGL::GL  # Changed from OpenGL::GL
)
