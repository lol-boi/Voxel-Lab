
# Voxel Engine Project

## Overview

This project is a 3D terrain renderer optimized with techniques like face culling, instancing, and greedy meshing. It efficiently renders voxel-based worlds with high performance and visual fidelity. Additionally, it includes a terrain generation system using FastNoiseLite for procedural landscapes.

## Features

- **3D Chunk Rendering**: Efficient rendering of large voxel-based worlds.
- **Instancing**: Reduces the number of draw calls for improved performance.
- **Face Culling**: Optimizes rendering by skipping hidden faces.
- **Greedy Meshing**: Minimizes geometry for better rendering performance.
- **Procedural Terrain Generation**: Generates diverse and interesting landscapes using FastNoiseLite.
- **Cross-Platform Support**: Designed to run on both Linux and Windows systems.
- **OpenGL Integration**: Utilizes modern OpenGL for rendering.

## Project Structure

```
project/
├── build/
├── src/
│   ├── libs/
│   │   ├── glad/
│   │   │   ├── include/
│   │   │   │   └── glad/
│   │   │   │       └── glad.h
│   │   │   └── src/
│   │   │       └── glad.c
│   │   ├── shader.hpp
│   │   └── shader.cpp
│   ├── application.cpp
│   └── CMakeLists.txt
```

## Dependencies

- **GLAD**: For loading OpenGL functions.
- **FastNoiseLite**: For terrain generation.
- **GLFW**: For creating windows and handling input.
- **GLM**: For mathematics operations (vectors, matrices, etc.).
- **CMake**: For building the project.

## Build Instructions

### Linux

1. Install dependencies:

   ```sh
   sudo pacman -S cmake glfw-x11 glm
   ```

2. Clone the repository:

   ```sh
   git clone <repository-url>
   cd project
   ```

3. Build the project:

   ```sh
   mkdir build
   cd build
   cmake ..
   make
   ```

4. Run the executable:

   ```sh
   ./voxel_engine
   ```

### Windows

1. Install dependencies (e.g., via vcpkg or other package manager).

2. Clone the repository:

   ```sh
   git clone <repository-url>
   cd project
   ```

3. Build the project:

   ```sh
   mkdir build
   cd build
   cmake -G "MinGW Makefiles" ..
   mingw32-make
   ```

4. Run the executable:

   ```sh
   ./voxel_engine.exe
   ```

## Usage

1. Execute the binary to start the voxel engine.
2. Use the WASD keys to navigate through the world.
3. Press the spacebar to jump and the mouse to look around.


## Screenshots

### 3D Terrain Rendering with Optimizations

![Screenshot 1](images/screenshot1.png)
![Screenshot 2](images/screenshot2.png)




## TODO

- **Complete the rendering using voxel pooling technique**: [High-Performance Voxel Engine](https://nickmcd.me/2021/04/04/high-performance-voxel-engine/).
  - Rework on the chunk delete and init logic.
  - Handle buckets efficiently(Smaller Fixed sized buckets or even some big size bucketes 2X).
  - Log chunk Init time.
  - Maybe chunk loading and Unloading from the Persistant storage.
- **Improve the texture assets**: Use solid colors instead of textures depending on the world height.
- **Enhance terrain generation**: Make the terrain generation more robust and feature-packed instead of just bland rendering.
- **Refactor and optimize**: Once the above tasks are completed, refactor the code, optimize memory usage, and add comments for clarity.


## Acknowledgments

- **GLAD** for OpenGL function loading.
- **FastNoiseLite** for noise generation.
- **GLFW** for window and input management.
- **GLM** for mathematics operations.
