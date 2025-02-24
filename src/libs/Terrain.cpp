#include "Terrain.hpp"
#include "glad/include/glad/glad.h"
#include <cstdlib>
#include <cwchar>
#include <glm/ext/vector_int3.hpp>
#include <glm/glm.hpp>
#include <mutex>
#include <vector>
#include "Chunk.hpp"
#include "imgui/imgui_impl_glfw.h"
#include <unordered_set>
#include <cmath>
#include <iostream>

Terrain::Terrain(int render_dist,int seed){
    chunk_size = 32;
    world_seed = seed;
    render_distance = render_dist;
    prev_chunk_pos = glm::ivec2(0,0);
    is_buffer_updated = false;
    vertices = {
        -0.5f, -0.5f, 0.5f,
         0.5f, -0.5f, 0.5f,
        -0.5f,  0.5f, 0.5f,
         0.5f,  0.5f, 0.5f
    };


    size_t max_chunks = (render_distance * 2 + 1) * (render_distance * 2 + 1) * 8; // x * z * y levels

    glGenBuffers(1,&indirect_buffer);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_buffer);

    glGenBuffers(1,&ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);

    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &gpu_mapped_ibo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(float)*3, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, gpu_mapped_ibo);
    glBufferStorage(GL_ARRAY_BUFFER, max_chunks * max_instances * sizeof(int), nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    instance_buffer_ptr = (int*)glMapBufferRange(GL_ARRAY_BUFFER, 0, max_chunks * max_instances * sizeof(GLuint), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    if (!instance_buffer_ptr) {
        std::cerr << "Failed to map instance buffer!" << std::endl;
    }

    for(int i = 0; i<max_chunks; i++){
        free_offsets.push(i);
    }

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

    glm::vec2 current_chunk_pos(x_chunk, z_chunk);

    if(prev_chunk_pos != glm::ivec2(x_chunk,z_chunk)){
        std::lock_guard<std::mutex> lock(buffer_mutex);

        chunk_positions.clear();

        for (auto it = chunks_data.begin(); it != chunks_data.end(); ) {
            if (glm::distance(glm::vec2(it->first.x, it->first.z), glm::vec2(x_chunk, z_chunk)) > render_distance) {
                auto pos =  it->first;
                free_offsets.push(chunk_offset_map[pos]);
                chunk_offset_map.erase(pos);
                delete it->second;
                it = chunks_data.erase(it);
            } else {
                ++it;
            }
        }

        for (int x = x_chunk - render_distance; x <= x_chunk + render_distance; x++) {
            for (int z = z_chunk - render_distance; z <= z_chunk + render_distance; z++) {
                for (int y = 0; y < 8; y++) {
                    glm::ivec3 pos(x, y, z);
                    if (!chunks_data.count(pos)) {
                        Chunk* chunk = new Chunk(pos);
                        chunk->gen_chunk_data(world_seed);
                        chunks_data[pos] = chunk;

                        unsigned int offset;
                        if(!free_offsets.empty()){
                            offset = free_offsets.front();
                            free_offsets.pop();
                            chunk_positions.push_back(glm::vec4(x, y, z, offset));
                            chunk_offset_map[pos] = offset;
                        }else{
                            offset = active_chunks.size();
                            std::cout << "No avalable buckets" << std::endl;
                        }

                    }
                }
            }
        }

        prev_chunk_pos = current_chunk_pos;

        update_buffer_data();
        return true;
    }
    return false;
}

void Terrain::update_buffer_data(){
    draw_commands.clear();

    for(auto &chunk : chunk_positions){
        (chunks_data[chunk])->cull_face(instance_buffer_ptr + (chunk_offset_map[glm::ivec3(chunk.x,chunk.y,chunk.z)] * max_instances));
        unsigned int gpu_instance_count = (chunks_data[chunk])->instance_count;

        DrawArraysIndirectCommand cmd;
        {
            cmd.count = 4;
            cmd.instanceCount = gpu_instance_count;
            cmd.first = 0;
            cmd.baseInstance = (chunk_offset_map[glm::ivec3(chunk.x,chunk.y,chunk.z)] * max_instances);  // Used to access the correct chunk position
        }
        //std::cout << instance_data.size() - offset_size << " " << gpu_instance_count << std::endl;
        draw_commands.push_back(cmd);

    }
    is_buffer_updated = true;
}

void Terrain:: upload_buffers(){
    std::lock_guard<std::mutex> lock(buffer_mutex);
    glBufferData(GL_SHADER_STORAGE_BUFFER, chunk_positions.size() * sizeof(glm::ivec4), chunk_positions.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

    glBufferData(GL_DRAW_INDIRECT_BUFFER, draw_commands.size() * sizeof(DrawArraysIndirectCommand),draw_commands.data(), GL_DYNAMIC_DRAW);

    is_buffer_updated = false;
}

void Terrain::draw_terrain(){
    glBindVertexArray(vao);
    glMultiDrawArraysIndirect(GL_TRIANGLE_STRIP, nullptr, draw_commands.size(),0);
}

int Terrain::chunk_count(){
    return chunk_positions.size();
}
