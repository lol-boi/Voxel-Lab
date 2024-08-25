#ifndef  TERRAIN_HPP
#define TERRAIN_HPP
#include <cmath>
#include "Chunk.hpp"
#include <vector>

class Terrain{
    int chunk_size;  //default val is 32
    int world_size_in_chunks;
    int world_seed;
    int world_size;

    public:
    Terrain(int no_of_chunks,int seed, int c_size);
    std::vector<Chunk> world_chunks;
    void init_world_chunks();
    void draw_terrain(unsigned int shader_program_id, int chunk_world_pos);
};

#endif
