#include "Terrain.hpp"
#include "../graphics/Renderer.hpp"
#include <cstdlib>
#include <cwchar>
#include "../external/glm/ext/vector_int3.hpp"
#include <queue>
#include "Chunk.hpp"
#include <cmath>
#include <iostream>
#include <cstring>


Terrain::Terrain(int render_dist,int seed){
    chunk_size = 32;
    world_seed = seed;
    render_distance = render_dist;
    prev_chunk_pos = glm::ivec2(0,0);


    bucket_size1 = 3 * chunk_size * chunk_size;
    max_chunks = (render_distance * 2 + 1) * (render_distance * 2 + 1) * 8; // x * z * y levels

    scene = new Render(max_chunks, bucket_size1);

    //init empty buckets
    for(int i = 0; i<max_chunks; i++){
        free_offsets1.push(i);
    }
}


bool Terrain::init_world_chunks(glm::vec3 cam_pos){


    int x_chunk = (int)cam_pos.x / chunk_size;
    int z_chunk = (int) cam_pos.z / chunk_size;
    glm::ivec2 current_chunk_pos(x_chunk, z_chunk);

    if(prev_chunk_pos == current_chunk_pos){
        return false;
    }

    // Remove out-of-range chunks
    for (auto it = chunks_data.begin(); it != chunks_data.end();) {
        glm::ivec3 pos = it->first;

        float x_dist = abs(pos.x-current_chunk_pos.x);
        float z_dist = abs(pos.z - current_chunk_pos.y);
        if (render_distance < x_dist || render_distance < z_dist) {
            unsigned int offset = chunk_offset_map[pos];

            // Mark buffers as unused
            scene->ssbo_ptr[offset] = glm::vec4(0.0f);
            scene->draw_command_ptr[offset].instanceCount = 0;
            free_offsets1.push(offset);

            // Cleanup
            delete it->second;
            chunk_offset_map.erase(pos);
            it = chunks_data.erase(it);
        } else {
            ++it;
        }
    }


    //Inserting new chunks and updating buffers for them(ssbo,index,indirect)
    for (int x = x_chunk - render_distance; x <= x_chunk + render_distance; ++x) {
        for (int z = z_chunk - render_distance; z <= z_chunk + render_distance; ++z) {
            for (int y = 0; y < 8; ++y) {

                glm::ivec3 pos(x, y, z);
                if (chunks_data.find(pos) != chunks_data.end()) continue; //if chunk is already present in chunks_data skip its initlization

                Chunk* chunk = new Chunk(pos);
                chunk->gen_chunk_data(world_seed);

                if(!free_offsets1.empty()){
                    unsigned int offset = free_offsets1.front();
                    free_offsets1.pop();

                    //buffer updates(instance_buffer, SSBO, Draw_commands)
                    chunk->cull_face(scene->instance_buffer_ptr + offset * bucket_size1);
                    scene->ssbo_ptr[offset] = glm::vec4(pos.x, pos.y, pos.z, 0);
                    scene->draw_command_ptr[offset] = {
                        4, // Vertices per instance
                        chunk->instance_count,
                        0, // First vertex
                        offset * bucket_size1
                    };
                    chunks_data[pos] = chunk;
                    chunk_offset_map[pos] = offset;

                }else{
                    std::cerr << "No empty buckets found!!" << std::endl;
                }

            }
        }
    }

    prev_chunk_pos = current_chunk_pos;
    return true;
}

void Terrain::draw() const{
    scene->draw(max_chunks);
}

Terrain::~Terrain(){
    delete scene;
    for (auto& [pos, chunk] : chunks_data) {
        delete chunk;
    }
}
