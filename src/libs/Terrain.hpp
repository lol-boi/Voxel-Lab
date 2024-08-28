#ifndef  TERRAIN_HPP
#define TERRAIN_HPP
#include <cmath>
#include "Chunk.hpp"
#include "glad/include/glad/glad.h"
#include <glm/ext/vector_float3.hpp>
#include <vector>


struct DrawArraysIndirectCommand {
    GLuint count;         // Number of vertices to draw
    GLuint instanceCount; // Number of instances to draw
    GLuint first;         // Starting index in the vertex array
    GLuint baseInstance;  // Starting index in the instance array
};

class Terrain{
    std::vector<float> vertices;
    unsigned int vbo, vao, ibo;
    int chunk_size;  //default val is 32
    int world_size_in_chunks;
    int world_seed;
    int world_size;
    unsigned int indirect_buffer;
    unsigned int ssbo;
    std::vector<DrawArraysIndirectCommand> draw_commands;
    std::vector<glm::vec3> chunk_positions;

    public:

    std::vector<int> instance_data;
    Terrain(int no_of_chunks,int seed, int c_size);
    std::vector<Chunk> world_chunks;
    void init_world_chunks();
    void draw_terrain();
    void update_terrain();
};

#endif
