#ifndef  TERRAIN_HPP
#define TERRAIN_HPP
#include <cmath>
#include "Chunk.hpp"
#include <vector>

class Terrain{
    std::vector<std::vector<int>> generation; // to store world gen data
    int chunk_size;
    int world_chunk_size;
    int world_size;


    public:
    Terrain(int c_size,int no_of_chunks);
    std::vector<Chunk> world_chunks;
    void gen_terrain(float freq1,float freq2,float freq3,float oct1,float oct2, float oct3);
    void init_world_chunks();
    void draw_terrain();
};

#endif
