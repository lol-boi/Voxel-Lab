#ifndef  TERRAIN_HPP
#define TERRAIN_HPP
#include <cmath>
#include "Chunk.hpp"
#include "glad/include/glad/glad.h"
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_int4.hpp>
#include <vector>
#include <unordered_map>


struct DrawArraysIndirectCommand {
    GLuint count;         // Number of vertices to draw
    GLuint instanceCount; // Number of instances to draw
    GLuint first;         // Starting index in the vertex array
    GLuint baseInstance;  // Starting index in the instance array
};

struct vec3Hash{
    std::size_t operator()(const::glm::vec3 &v)const{
        return std::hash<float>()(v.x) ^ std::hash<float>()(v.y) ^ std::hash<float>()(v.z);
    }
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
    std::vector<glm::vec4> chunk_positions;
    glm::vec2 prev_pos;

    public:

    std::vector<int> instance_data;
    Terrain(int no_of_chunks,int seed, int c_size);
    std::unordered_map<glm::vec3,Chunk, vec3Hash> chunkmap;
    void init_world_chunks(glm::vec3);
    void draw_terrain();
    void update_terrain();
};

#endif
