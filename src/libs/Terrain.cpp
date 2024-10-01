#include "Terrain.hpp"
#include "glad/include/glad/glad.h"
#include <cstdlib>
#include <cwchar>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_int2.hpp>
#include <mutex>
#include <vector>
#include "Chunk.hpp"
#include <cmath>
#include <iostream>

Terrain::Terrain(int no_of_chunks,int seed,  int c_size = 32){
    no_of_chunks += 1;
    chunk_size = c_size;
    world_seed = seed;
    render_distance = 3;
    prev_chunk_pos = glm::ivec2(0,0);
    is_buffer_updated = false;
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

    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
}


bool Terrain::init_world_chunks(glm::vec3 cam_pos){
    int x_chunk = (int)cam_pos.x / 32;
    int z_chunk = (int) cam_pos.z / 32;

    if(prev_chunk_pos != glm::ivec2(x_chunk,z_chunk)){
        std::cout << "Player now at [" << x_chunk << "," << z_chunk<< "]" <<std::endl;
        std::lock_guard<std::mutex> lock(buffer_mutex);
        chunk_positions.clear();
        chunks_data.clear();
        for(int x = x_chunk - render_distance; x <= x_chunk + render_distance; x++ ){
            for(int z = z_chunk - render_distance; z <= z_chunk + render_distance; z++){
                for(int y = 0; y<8; y++){
                    Chunk chunk = Chunk(glm::vec3(x,y,z));
                    chunk.gen_chunk_data(chunk_size, world_seed);
                    chunks_data.push_back(chunk);
                    chunk_positions.push_back(glm::vec4(x,y,z,0));
                }
            }
        }
        prev_chunk_pos = glm::vec2(x_chunk,z_chunk);
        update_buffer_data();
        return true;
    }
    return false;
}

void Terrain::update_buffer_data(){
    draw_commands.clear();
    instance_data.clear();

    size_t offset_size = 0;

    for(auto &chunk : chunks_data){
        chunk.gen_mesh(chunk_size, &instance_data);
        DrawArraysIndirectCommand cmd;
        {
            cmd.count = 4;
            cmd.instanceCount = instance_data.size() - offset_size;
            cmd.first = 0;
            cmd.baseInstance = offset_size; // Used to access the correct chunk position
        }
        draw_commands.push_back(cmd);
        offset_size = instance_data.size();
    }
    is_buffer_updated = true;
}

void Terrain:: upload_buffers(){
    std::lock_guard<std::mutex> lock(buffer_mutex);
    glBufferData(GL_SHADER_STORAGE_BUFFER, chunk_positions.size() * sizeof(glm::vec4), chunk_positions.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

    glBufferData(GL_DRAW_INDIRECT_BUFFER, draw_commands.size() * sizeof(DrawArraysIndirectCommand),draw_commands.data(), GL_DYNAMIC_DRAW);

    glBufferData(GL_ARRAY_BUFFER,instance_data.size() * sizeof(int), instance_data.data(),GL_DYNAMIC_DRAW);
    std::cout << "Uploaded the buffers" << std::endl;
    is_buffer_updated = false;
}

void Terrain::draw_terrain(){
    glBindVertexArray(vao);
    glMultiDrawArraysIndirect(GL_TRIANGLE_STRIP, nullptr, draw_commands.size(),0);
}

int Terrain::chunk_count(){
    return chunk_positions.size();
}
