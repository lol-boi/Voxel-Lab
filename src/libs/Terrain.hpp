#ifndef  TERRAIN_HPP
#define TERRAIN_HPP
#include <cmath>
#include "Chunk.hpp"
#include "glad/include/glad/glad.h"
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_int2.hpp>
#include <glm/ext/vector_int4.hpp>
#include <mutex>
#include <vector>


struct DrawArraysIndirectCommand {
    GLuint count;         // Number of vertices to draw
    GLuint instanceCount; // Number of instances to draw
    GLuint first;         // Starting index in the vertex array
    GLuint baseInstance;
    bool operator==(const DrawArraysIndirectCommand& other) const {
        return count == other.count &&
               instanceCount == other.instanceCount &&
               first == other.first &&
               baseInstance == other.baseInstance;
    } // Starting index in the instance array
};

class Terrain{
    public:
        Terrain(int no_of_chunks,int seed);

        bool is_buffer_updated;
        int render_distance;
        std::vector<int> instance_data;

        bool init_world_chunks(glm::vec3);
        void update_buffer_data();
        void update_chunk(); // When we destroy or insert a block
        void draw_terrain();
        void upload_buffers();
        int chunk_count();

    private:
        std::vector<float> vertices;
        unsigned int vbo, vao, ibo;
        unsigned int indirect_buffer;
        unsigned int ssbo;

        int chunk_size;
        int world_seed;
        glm::ivec2 prev_chunk_pos;
        std::mutex buffer_mutex;

        std::vector<DrawArraysIndirectCommand> draw_commands;
        std::vector<glm::vec4> chunk_positions;
        std::vector<Chunk*> chunks_data;
};

#endif
