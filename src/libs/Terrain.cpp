#include "Terrain.hpp"
#include "glad/include/glad/glad.h"
#include <cstdlib>
#include <cwchar>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_int2.hpp>
#include <unordered_map>
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
    prev_pos = glm::vec2(0,0);
    vertices = {
        -0.5f, -0.5f, 0.5f,
         0.5f, -0.5f, 0.5f,
        -0.5f,  0.5f, 0.5f,
         0.5f,  0.5f, 0.5f
    };

    glGenBuffers(1,&indirect_buffer);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_buffer);

    glGenBuffers(1,&ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);

    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ibo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(float)*3, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, ibo);
    glVertexAttribIPointer(1,1,GL_UNSIGNED_INT,sizeof(GLuint),(void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1,1);

    glBindVertexArray(0);
}


void Terrain::init_world_chunks(glm::vec3 cam_pos){
    chunk_positions.clear();
    int render_distance = 4;
    int x_chunk, z_chunk;
    x_chunk = (int)cam_pos.x / 32;
    z_chunk = (int) cam_pos.y / 32;
    if(prev_pos == glm::vec2(x_chunk,z_chunk)){
       return;
    }
    for(int x = x_chunk - render_distance; x <= x_chunk + render_distance; x++ ){
        for(int z = z_chunk - render_distance; z <= z_chunk + render_distance; z++){
            for(int y = 0; y<8; y++){
                Chunk chunk = Chunk(glm::vec3(x,y,z));
                chunk.gen_chunk_data(chunk_size, world_seed);
                chunkmap.emplace(glm::vec3(x,y,z), chunk);
                chunk_positions.push_back(glm::vec4(x,y,z,0));
            }
        }
    }
    //for(int x = 0; x<world_size_in_chunks; x++){
    //    for(int y = 0; y<8; y++){
    //        for(int z = 0; z<world_size_in_chunks; z++){
    //           Chunk chunk = Chunk(glm::vec3(x,y,z));
    //           chunk.gen_chunk_data(chunk_size,world_seed);
    //           chunkmap.emplace(glm::vec3(x,y,z), chunk);
    //        }
    //    }
    //}
}

void Terrain::update_terrain(){
    draw_commands.clear();
    instance_data.clear();

    int offset_size = 0;

    for(auto &entry: chunk_positions){

        Chunk &chunk = chunkmap.at(glm::vec3(entry.x,entry.y,entry.z));
        chunk.gen_mesh(chunk_size, &instance_data);

        DrawArraysIndirectCommand cmd;
        cmd.count = 4;
        cmd.instanceCount = instance_data.size() - offset_size;
        cmd.first = 0;
        cmd.baseInstance = offset_size; // Used to access the correct chunk position
        draw_commands.push_back(cmd);
        offset_size = instance_data.size();
    }
    glBufferData(GL_SHADER_STORAGE_BUFFER, chunk_positions.size() * sizeof(glm::vec4), chunk_positions.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

    glBufferData(GL_DRAW_INDIRECT_BUFFER, draw_commands.size() * sizeof(DrawArraysIndirectCommand),draw_commands.data(), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER,ibo);
    glBufferData(GL_ARRAY_BUFFER,instance_data.size() * sizeof(int), instance_data.data(),GL_STATIC_DRAW);
}

void Terrain::draw_terrain(){
    glBindVertexArray(vao);
    glMultiDrawArraysIndirect(GL_TRIANGLE_STRIP, nullptr, draw_commands.size(),0);
}
