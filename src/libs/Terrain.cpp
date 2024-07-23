#include "Terrain.hpp"
#include <cstdlib>
#include <cwchar>
#include <vector>
#include "Chunk.hpp"
#include "FastNoiseLite.h"
#include <cmath>


Terrain::Terrain( int c_size, int no_of_chunks){
   world_chunk_size  = no_of_chunks;
   chunk_size = c_size;
   world_size = chunk_size * no_of_chunks;
}

void Terrain::gen_terrain(float freq1,float freq2,float freq3, float oct1, float oct2, float oct3){
    //int seed;//random seed
    FastNoiseLite noise;
    generation.clear();
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    for(int i = (-1)*((world_chunk_size/2)*chunk_size); i<(world_chunk_size/2)*chunk_size; i++){
        std::vector<int> tmp;
        for(int j = (-1)*((world_chunk_size/2)*chunk_size); j< (world_chunk_size/2) * chunk_size; j ++){
            float height =
                    oct1 * noise.GetNoise(freq1*(float)i,freq1*(float)j) +
                    oct2 * noise.GetNoise(freq2*(float)i,freq2*(float)j) +
                    oct3 * noise.GetNoise(freq3*(float)i,freq3*(float)j);
            height = (int)((height+1)*9.75 + 60);
            tmp.push_back(height);
        }
        generation.push_back(tmp);
    }
}

void Terrain::init_world_chunks(){
    world_chunks.clear();
    for(int cx = 0; cx<world_chunk_size;cx++){
        for(int cy = 0; cy<world_chunk_size; cy++){
            std::vector<glm::vec3> chunk_positions;
            for(int x = 0; x<chunk_size; ++x){
                for(int z = 0; z<chunk_size; ++z){
                    int height = generation[(cx*chunk_size)+x][(cy*chunk_size)+z];
                    glm::vec3 pos((cx*chunk_size)+x,height,(cy*chunk_size)+z);
                    for(int y = 0; y<=height; y++){
                        chunk_positions.push_back(pos);
                    }
                }
            }
            Chunk c;
            c.gen_chunk_data(chunk_positions);
            world_chunks.push_back(c);
        }
    }
}
void Terrain::draw_terrain(){
    for(Chunk &chunk : world_chunks){
        chunk.draw();
    }
}
