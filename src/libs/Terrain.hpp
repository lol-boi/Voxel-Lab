#ifndef  TERRAIN_HPP
#define TERRAIN_HPP
#include <cmath>
#include "Chunk.hpp"
#include "Renderer.hpp"
#include <glm/glm.hpp>
#include <queue>
#include <unordered_map>

struct ivec3_hash {
    std::size_t operator()(const glm::ivec3& v) const {
        std::size_t h1 = std::hash<int>()(v.x);
        std::size_t h2 = std::hash<int>()(v.y);
        std::size_t h3 = std::hash<int>()(v.z);

        // Use large prime numbers for better bit spreading
        return h1 ^ (h2 * 73856093) ^ (h3 * 19349663);
    }
};

class Terrain{
    public:
        int render_distance;

        Terrain(int no_of_chunks,int seed);
        ~Terrain();

        bool init_world_chunks(glm::vec3);
        void draw();

    private:
        int bucket_size1; // (max_instances / 2)
        int max_chunks;
        int chunk_size;
        int world_seed;
        glm::ivec2 prev_chunk_pos;
        Render * scene;

        std::unordered_map<glm::ivec3, Chunk*,ivec3_hash> chunks_data;
        std::unordered_map<glm::ivec3, int,ivec3_hash> chunk_offset_map;
        std::queue<int> free_offsets1;
};

#endif
