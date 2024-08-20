#ifndef  TERRAIN_HPP
#define TERRAIN_HPP
#include <cmath>
#include "Chunk.hpp"
#include <vector>

class Terrain{
    int chunk_size;  //default val is 32
    int world_size_in_chunks;
    int world_seed;
    //its value is in one dimention so the final world will be of world_size_in_chunks * 8 * world_size_in_chunks
    //val of world_size_in_chunks should be even if not 1 will be added to make it even
    int world_size;

    public:
    Terrain(int no_of_chunks,int seed,int c_size);
    std::vector<Chunk> world_chunks;
    void init_world_chunks();
    void draw_terrain();
};

#endif
