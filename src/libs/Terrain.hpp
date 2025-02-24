#ifndef  TERRAIN_HPP
#define TERRAIN_HPP
#include <cmath>
#include "Chunk.hpp"
#include "glad/include/glad/glad.h"
#include <glm/glm.hpp>
#include <mutex>
#include <queue>
#include <unordered_map>
#include <vector>

struct ivec3_hash {
    std::size_t operator()(const glm::ivec3& v) const {
        std::size_t h1 = std::hash<int>()(v.x);
        std::size_t h2 = std::hash<int>()(v.y);
        std::size_t h3 = std::hash<int>()(v.z);

        // Use large prime numbers for better bit spreading
        return h1 ^ (h2 * 73856093) ^ (h3 * 19349663);
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

        bool init_world_chunks(glm::vec3);
        void update_buffer_data();
        void update_chunk(); // When we destroy or insert a block
        void draw_terrain();
        void upload_buffers();
        int chunk_count();

    private:
        std::vector<float> vertices;
        unsigned int vbo, vao, gpu_mapped_ibo;
        unsigned int indirect_buffer;
        unsigned int ssbo;
        int * instance_buffer_ptr;

        int chunk_size;
        int world_seed;
        glm::ivec2 prev_chunk_pos;
        std::mutex buffer_mutex;

        std::vector<DrawArraysIndirectCommand> draw_commands;
        std::vector<glm::ivec4> chunk_positions;
        std::unordered_map<glm::ivec3, Chunk*,ivec3_hash> chunks_data;

        std::unordered_map<glm::ivec3, int,ivec3_hash> chunk_offset_map;
        std::queue<int> free_offsets;
        std::vector<glm::ivec3> active_chunks;
        int max_instances = 6 *32 * 32;
};

#endif
