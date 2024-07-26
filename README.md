# Voxel Engine Project

## Overview

This project is a voxel-based engine designed for rendering 3D chunks of blocks. The engine is optimized for performance and visual fidelity through techniques like instancing and face culling. Additionally, it includes a terrain generation system using FastNoiseLite for procedural landscapes.

## Features

- **3D Chunk Rendering**: Efficient rendering of large voxel-based worlds.
- **Instancing**: Reduces the number of draw calls for improved performance.
- **Face Culling**: Optimizes rendering by skipping hidden faces.
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

## Contributing

1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Commit your changes (`git commit -am 'Add new feature'`).
4. Push to the branch (`git push origin feature-branch`).
5. Create a new Pull Request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgments

- **GLAD** for OpenGL function loading.
- **FastNoiseLite** for noise generation.
- **GLFW** for window and input management.
- **GLM** for mathematics operations.

## Contact

For any questions or feedback, please contact [your-email@example.com].
