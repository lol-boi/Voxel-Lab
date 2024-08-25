
#include "Terrain.hpp"
#include "glad/include/glad/glad.h"
#include <cstdlib>
#include <cwchar>
#include <vector>
#include "Chunk.hpp"
#include <cmath>


Terrain::Terrain(int no_of_chunks,int seed,  int c_size = 32){
    if(no_of_chunks % 2 != 0){
        no_of_chunks += 1;
    }
    world_size_in_chunks  = no_of_chunks;
    chunk_size = c_size;
    world_size = chunk_size * no_of_chunks;
    world_seed = seed;
}


void Terrain::init_world_chunks(){
    for(int x = 0; x<world_size_in_chunks; x++){
        for(int y = 0; y<8; y++){
            for(int z = 0; z<world_size_in_chunks; z++){
               Chunk chunk = Chunk(glm::vec3(x,y,z));
               chunk.gen_chunk_data(chunk_size,world_seed);
               world_chunks.push_back(chunk);
            }
        }
    }
}
void Terrain::draw_terrain(unsigned int shader_program_id, int chunk_world_pos){
    for(Chunk &chunk : world_chunks){
        glUseProgram(shader_program_id);
        glUniform3f(chunk_world_pos, chunk.chunk_pos_in_world.x, chunk.chunk_pos_in_world.y, chunk.chunk_pos_in_world.z);
        chunk.draw();
    }
}
