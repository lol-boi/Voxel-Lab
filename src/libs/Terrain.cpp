#include "Terrain.hpp"
#include "glad/include/glad/glad.h"
#include <cstdlib>
#include <cwchar>
#include <glm/ext/vector_int3.hpp>
#include <glm/glm.hpp>
#include <mutex>
#include <vector>
#include "Chunk.hpp"
#include <cmath>
#include <iostream>
#include <cstring>


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


    max_chunks = (render_distance * 2 + 1) * (render_distance * 2 + 1) * 8; // x * z * y levels

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


    glVertexAttribIPointer(1,1,GL_UNSIGNED_INT,sizeof(GLuint),(void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1,1);

    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_buffer);
    glBufferStorage(GL_DRAW_INDIRECT_BUFFER, max_chunks * sizeof(DrawArraysIndirectCommand), nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    draw_command_ptr = (DrawArraysIndirectCommand*)glMapBufferRange(GL_DRAW_INDIRECT_BUFFER, 0, max_chunks * sizeof(DrawArraysIndirectCommand), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, max_chunks * sizeof(glm::vec4), nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    ssbo_ptr = (glm::vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, max_chunks * sizeof(glm::vec4), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    memset(ssbo_ptr, 0, max_chunks * sizeof(glm::vec4));

    for(int i = 0; i<max_chunks; i++){
        free_offsets.push(i);
    }

    glBindVertexArray(0);
}


bool Terrain::init_world_chunks(glm::vec3 cam_pos){


    int x_chunk = (int)cam_pos.x / chunk_size;
    int z_chunk = (int) cam_pos.z / chunk_size;
    glm::ivec2 current_chunk_pos(x_chunk, z_chunk);

    if(prev_chunk_pos == current_chunk_pos){
        return false;
    }

    std::lock_guard<std::mutex> lock(buffer_mutex);

    // Remove out-of-range chunks
    for (auto it = chunks_data.begin(); it != chunks_data.end();) {
        glm::ivec3 pos = it->first;

        float x_dist = abs(pos.x-current_chunk_pos.x);
        float z_dist = abs(pos.z - current_chunk_pos.y);
        if (render_distance < x_dist || render_distance < z_dist) {
            unsigned int offset = chunk_offset_map[pos];

            // Mark buffers as unused
            ssbo_ptr[offset] = glm::vec4(0.0f);
            draw_command_ptr[offset].instanceCount = 0;
            free_offsets.push(offset);

            // Cleanup
            delete it->second;
            chunk_offset_map.erase(pos);
            it = chunks_data.erase(it);
        } else {
            ++it;
        }
    }

    //Inserting new chunks and updating buffers for them(ssbo,index,indirect)

    int largest_instance = 0;
    for (int x = x_chunk - render_distance; x <= x_chunk + render_distance; ++x) {
        for (int z = z_chunk - render_distance; z <= z_chunk + render_distance; ++z) {
            for (int y = 0; y < 8; ++y) {
                glm::ivec3 pos(x, y, z);
                if (chunks_data.find(pos) != chunks_data.end())
                    continue;

                if (free_offsets.empty()) {
                    std::cerr << "No free chunk slots available!" << std::endl;
                    continue;
                }

                unsigned int offset = free_offsets.front();
                free_offsets.pop();

                // Initialize chunk
                Chunk* chunk = new Chunk(pos);
                chunk->gen_chunk_data(world_seed);
                chunk->cull_face(instance_buffer_ptr + offset * max_instances);
                {
                    if(chunk->instance_count > largest_instance){
                        largest_instance = chunk->instance_count;
                    }
                }
                // Update buffers
                ssbo_ptr[offset] = glm::vec4(pos.x, pos.y, pos.z, 0);
                draw_command_ptr[offset] = {
                    4, // Vertices per instance
                    chunk->instance_count,
                    0, // First vertex
                    offset * max_instances
                };

                chunks_data[pos] = chunk;
                chunk_offset_map[pos] = offset;
            }
        }
    }
   {
       std::cout << largest_instance << std::endl;
   }

    prev_chunk_pos = current_chunk_pos;
    return true;
}



void Terrain::draw_terrain(){

    glBindVertexArray(vao);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
    glMultiDrawArraysIndirect(GL_TRIANGLE_STRIP, nullptr, max_chunks, 0);
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << err << std::endl;
    }
}

int Terrain::chunk_count(){
    return chunk_positions.size();
}
