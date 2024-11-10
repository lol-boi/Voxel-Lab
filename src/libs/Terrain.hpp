#ifndef  TERRAIN_HPP
#define TERRAIN_HPP
#include <cmath>
#include "Chunk.hpp"
#include "glad/include/glad/glad.h"
#include <glm/glm.hpp>
#include <mutex>
#include <unordered_map>
#include <vector>

struct ivec3_hash {
    std::size_t operator()(const glm::ivec3& v) const noexcept {
        std::hash<int> hasher;
        std::size_t seed = 0;
        seed ^= hasher(v.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hasher(v.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hasher(v.z) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};
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
        std::unordered_map<glm::ivec3, Chunk*,ivec3_hash> chunks_data;
};

#endif
