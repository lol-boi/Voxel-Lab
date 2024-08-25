#include "Chunk.hpp"
#include "glad/include/glad/glad.h"
#include "FastNoiseLite.h"
#include <bits/types/clockid_t.h>
#include <glm/ext/vector_float3.hpp>
#include <vector>


const int faceIndices[6] = {
     0, 1, 2, 2, 3, 0
};

Chunk::Chunk(glm::vec3 pos) {
    chunk_pos_in_world = pos;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribIPointer(0, 1, GL_INT, sizeof(int), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBindVertexArray(0);
}

void Chunk::gen_chunk_data(int c_size, int seed){

    chunk_data = new int[c_size*c_size*c_size];
    float oct1 = .5f, oct2 = .8f, oct3 = 1.0f;
    float freq1 = 1.0f, freq2 = 1.25f, freq3  = 1.5f;

    FastNoiseLite noise(seed);
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    for(int x = 0; x<c_size; x++){
        for(int z = 0; z<c_size;  z++){

           int a,b;
           a = (chunk_pos_in_world.x * c_size) + x;
           b = (chunk_pos_in_world.z * c_size) + z;

            float height =
                  oct1 * noise.GetNoise(freq1*(float)a,freq1*(float)b) +
                    oct2 * noise.GetNoise(freq2*(float)a,freq2*(float)b) +
                    oct3 * noise.GetNoise(freq3*(float)a,freq3*(float)b);

             height = height /(oct1 + oct2 + oct3);
             height = (int) ((height+1) * 256/2);
            if(height < 60) height  = 60;

            for(int y = (c_size*chunk_pos_in_world.y); y<(c_size * (chunk_pos_in_world.y + 1)); y++){
                int index = (c_size * c_size * (y-(c_size * chunk_pos_in_world.y))) + (c_size * z) + x;
                if(height >= y){
                    if(y == 1){
                       chunk_data[index] = 4;
                    }
                    else if(y == height){
                        chunk_data[index] = 1;
                    }
                    else if (y < height && y >= height - 4){
                        chunk_data[index] = 3;
                    }
                    else if(y < height -4 && y>1){
                        chunk_data[index] = 2;
                    }
                    else{
                        chunk_data[index] = 4;
                    }
                }
                else{
                    chunk_data[index] = 0;
                }
            }
        }
    }
    gen_mesh(c_size);
}

int Chunk:: pack_data(int x, int y, int z, int normal, int texture,int u, int v) {
    int data = 0;
    data |= (x & 63);
    data |= (y & 63) << 6;
    data |= (z & 63) << 12;
    data |= (normal & 7) << 18;
    data |= (texture & 15) << 21;
    data |= (u & 1) << 22;
    data |= (v & 1) << 23;
    return data;
}

void Chunk::gen_mesh(int c_size) {
    // Clear previous data
    vertices.clear();
    indices.clear();

   for(int z = 0; z<c_size; z++){
       for(int y = 0; y<c_size; y++){
           for(int x = 0; x<c_size; x++){
               int index = (c_size*c_size*y) + (c_size * z) + x;

               if(chunk_data[index]){
                   int N = chunk_data[index];
                   bool left = (x > 0) ? !chunk_data[index - 1] : true;
                   bool right = (x < c_size-1) ? !chunk_data[index + 1] : true;
                   bool front = (z > 0) ? !chunk_data[index - c_size] : true;
                   bool back = (z < c_size-1) ? !chunk_data[index + c_size] : true;
                   bool top = (y < c_size-1 ) ? !chunk_data[index + (c_size * c_size)] : true;
                   bool bottom = (y > 0) ? !chunk_data[index - (c_size * c_size)] : true;

                   int face[4];

                   if(left){

                        face[0] = pack_data(x,y,z+1,0,N,0,0);
                        face[1] = pack_data(x,y,z,0,N,1,0);
                        face[2] = pack_data(x,y+1,z,0,N,1,1);
                        face[3] = pack_data(x,y+1,z+1,0,N,0,1);

                        int base_index = vertices.size();
                        for(int i = 0; i<4; i++){
                            vertices.push_back(face[i]);
                        }
                        for(int i = 0; i<6; ++i){
                           indices.push_back(faceIndices[i] + base_index);
                        }
                   }
                   if(right){



                       face[0] = pack_data(x+1,y,z+0,1,N,0,0);
                       face[1] = pack_data(x+1,y,z+1,1,N,1,0);
                       face[2] = pack_data(x+1,y+1,z+1,1,N,1,1);
                       face[3] = pack_data(x+1,y+1,z,1,N,0,1);

                       int base_index = vertices.size();

                       for(int i = 0; i<4; i++){
                           vertices.push_back(face[i]);
                       }
                       for(int i = 0; i<6; ++i){
                           indices.push_back(faceIndices[i] + base_index);
                       }
                   }


                   if(back){



                       face[0] = pack_data(x+1,y,z+1,2,N,0,0);
                       face[1] = pack_data(x,y,z+1,2,N,1,0);
                       face[2] = pack_data(x,y+1,z+1,2,N,1,1);
                       face[3] = pack_data(x+1,y+1,z+1,2,N,0,1);

                       int base_index = vertices.size();
                       for(int i = 0; i<4; i++){
                            vertices.push_back(face[i]);
                        }
                        for(int i = 0; i<6; ++i){
                            indices.push_back(faceIndices[i] + base_index);
                        }
                    }
                   if(front){


                       face[0] = pack_data(x,y,z,3,N,0,0);
                       face[1] = pack_data(x+1,y,z,3,N,1,0);
                       face[2] = pack_data(x+1,y+1,z,3,N,1,1);
                       face[3] = pack_data(x,y+1,z,3,N,0,1);

                       int base_index = vertices.size();
                       for(int i = 0; i<4; i++){
                           vertices.push_back(face[i]);
                       }
                       for(int i = 0; i<6; ++i){
                           indices.push_back(faceIndices[i] + base_index);
                       }
                   }
                   if(top){


                       face[0] = pack_data(x,y+1,z,4,N,0,0);
                       face[1] = pack_data(x+1,y+1,z,4,N,1,0);
                       face[2] = pack_data(x+1,y+1,z+1,4,N,1,1);
                       face[3] = pack_data(x,y+1,z+1,4,N,0,1);

                       int base_index = vertices.size();
                       for(int i = 0; i<4; i++){
                           vertices.push_back(face[i]);
                       }
                       for(int i = 0; i<6; ++i){
                           indices.push_back(faceIndices[i] + base_index);
                       }
                   }
                   if(bottom){


                       face[0] = pack_data(x+1,y,z,5,N,0,0);
                       face[1] = pack_data(x,y,z,5,N,1,0);
                       face[2] = pack_data(x,y,z+1,5,N,1,1);
                       face[3] = pack_data(x+1,y,z+1,5,N,0,1);

                       int base_index = vertices.size();
                       for(int i = 0; i<4; i++){
                           vertices.push_back(face[i]);
                       }
                       for(int i = 0; i<6; ++i){
                           indices.push_back(faceIndices[i] + base_index);
                       }
                   }
                }
           }
       }
   }

    // Update VBO and EBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(int), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Chunk::draw() {
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
