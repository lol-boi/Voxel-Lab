#include "Chunk.hpp"
#include "glad/include/glad/glad.h"
#include "FastNoiseLite.h"
#include <glm/ext/vector_float3.hpp>
#include <vector>

// Indices for the six faces of a cube
const int faceIndices[6][6] = {
    { 0, 1, 2, 2, 3, 0 }, // Left
    { 0, 1, 2, 2, 3, 0 }, // Right
    { 0, 1, 2, 2, 3, 0 }, // Bottom
    { 0, 1, 2, 2, 3, 0 }, // Top
    { 0, 1, 2, 2, 3, 0 }, // Back
    { 0, 1, 2, 2, 3, 0 }  // Front
};



Chunk::Chunk(glm::vec3 pos) {
    chunk_pos_in_world = pos;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBindVertexArray(0);
}

//Chunk::~Chunk(){
//    delete[] chunk_data;
//}
void Chunk::gen_chunk_data(int c_size, int seed){
    chunk_data = new int[c_size * c_size * c_size];
    FastNoiseLite noise(seed);
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    for(int x = 0; x<c_size; x++){
        for(int z = 0; z<c_size;  z++){
            int height = noise.GetNoise((chunk_pos_in_world.x * c_size) + x , (chunk_pos_in_world.z * c_size) + z);
            height = ((height + 1) * c_size * 8) / 2;
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
            //std::cout << height << " "; //debug
        }
        //std::cout << std::endl; //debug
    }
    gen_mesh(c_size);
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

                   int a,b,c;
                   a = x + (chunk_pos_in_world.x * c_size);
                   b = y + (chunk_pos_in_world.y * c_size);
                   c = z + (chunk_pos_in_world.z * c_size);
                   if(left){ //left
                        float face[] = {
                           -0.5f + a, -0.5f + b, -0.5f + c, 0.75f, (N-1) * 0.0625f,
                           -0.5f + a, -0.5f + b,  0.5f + c,  1.0f, (N-1) * 0.0625f,
                           -0.5f + a,  0.5f + b,  0.5f + c,  1.0f,  (N) * 0.0625f,
                           -0.5f + a,  0.5f + b, -0.5f + c,  0.75f, (N) * 0.0625f };

                        for(int i = 0; i<20; i+=5){
                            vertices.push_back(face[i+0]);
                            vertices.push_back(face[i+1]);
                            vertices.push_back(face[i+2]);
                            vertices.push_back(face[i+3]);
                            vertices.push_back(face[i+4]);
                        }
                        int base_index = vertices.size()/ 5 - 4;
                        for(int i = 0; i<6; ++i){
                           indices.push_back(faceIndices[0][i] + base_index);
                        }
                   }
                   if(right){ //right
                       float face[] = {
                           0.5f + a, -0.5f + b,  0.5f + c, 0.75f, (N-1) * 0.0625f,
                           0.5f + a, -0.5f + b, -0.5f + c,  1.0f, (N-1) * 0.0625f,
                           0.5f + a,  0.5f + b, -0.5f + c,  1.0f,  (N) * 0.0625f,
                           0.5f + a,  0.5f + b,  0.5f + c,  0.75f, (N) * 0.0625f };

                       for(int i = 0; i<20; i+=5){
                           vertices.push_back(face[i+0]);
                           vertices.push_back(face[i+1]);
                           vertices.push_back(face[i+2]);
                           vertices.push_back(face[i+3]);
                           vertices.push_back(face[i+4]);
                       }
                       int base_index = vertices.size()/ 5 - 4;
                       for(int i = 0; i<6; ++i){
                           indices.push_back(faceIndices[1][i] + base_index);
                       }
                   }
                   if(back){ //front
                        float face[] = {
                        -0.5f + a, -0.5f + b, 0.5f + c, 0.5f, (N-1) * 0.0625f,
                         0.5f + a, -0.5f + b, 0.5f + c, 0.75f,  (N-1) * 0.0625f,
                         0.5f + a,  0.5f + b, 0.5f + c, 0.75f, (N) * 0.0625f,
                        -0.5f + a,  0.5f + b, 0.5f + c, 0.5f, (N) * 0.0625f };
                        for(int i = 0; i<20; i+=5){
                            vertices.push_back(face[i+0]);
                            vertices.push_back(face[i+1]);
                            vertices.push_back(face[i+2]);
                            vertices.push_back(face[i+3]);
                            vertices.push_back(face[i+4]);
                        }
                        int base_index = vertices.size()/ 5 - 4;
                        for(int i = 0; i<6; ++i){
                            indices.push_back(faceIndices[5][i] + base_index);
                        }
                    }
                   if(front){ //back
                       float face[] = {
                           0.5f + a, -0.5f + b, -0.5f + c, 0.5f, (N-1) * 0.0625f,
                          -0.5f + a, -0.5f + b, -0.5f + c, 0.75f, (N-1) * 0.0625f,
                          -0.5f + a,  0.5f + b, -0.5f + c, 0.75f, (N) * 0.0625f,
                           0.5f + a,  0.5f + b, -0.5f + c, 0.5f, (N) * 0.0625f };
                       for(int i = 0; i<20; i+=5){
                           vertices.push_back(face[i+0]);
                           vertices.push_back(face[i+1]);
                           vertices.push_back(face[i+2]);
                           vertices.push_back(face[i+3]);
                           vertices.push_back(face[i+4]);
                       }
                       int base_index = vertices.size()/ 5 - 4;
                       for(int i = 0; i<6; ++i){
                           indices.push_back(faceIndices[4][i] + base_index);
                       }
                   }
                   if(top){ //top
                       float face[] = {
                       -0.5f + a, 0.5f + b,  0.5f + c, 0.0f, (N-1) * 0.0625f,
                        0.5f + a, 0.5f + b,  0.5f + c, 0.25f,  (N-1) * 0.0625f,
                        0.5f + a, 0.5f + b, -0.5f + c, 0.25f,  N * 0.0625f,
                       -0.5f + a, 0.5f + b, -0.5f + c, 0.0f, N * 0.0625f };
                       for(int i = 0; i<20; i+=5){
                           vertices.push_back(face[i+0]);
                           vertices.push_back(face[i+1]);
                           vertices.push_back(face[i+2]);
                           vertices.push_back(face[i+3]);
                           vertices.push_back(face[i+4]);
                       }
                       int base_index = vertices.size()/ 5 - 4;
                       for(int i = 0; i<6; ++i){
                           indices.push_back(faceIndices[3][i] + base_index);
                       }
                   }
                   if(bottom){  //bottom
                       float face[] = {
                           0.5f + a, -0.5f + b,  0.5f + c, 0.25f, (N-1) * 0.0625f,
                          -0.5f + a, -0.5f + b,  0.5f + c, 0.5f,  (N-1) * 0.0625f,
                          -0.5f + a, -0.5f + b, -0.5f + c, 0.5f,  N * 0.0625f,
                           0.5f + a, -0.5f + b, -0.5f + c, 0.25f, N * 0.0625f };
                       for(int i = 0; i<20; i+=5){
                           vertices.push_back(face[i+0]);
                           vertices.push_back(face[i+1]);
                           vertices.push_back(face[i+2]);
                           vertices.push_back(face[i+3]);
                           vertices.push_back(face[i+4]);
                       }
                       int base_index = vertices.size()/ 5 - 4;
                       for(int i = 0; i<6; ++i){
                           indices.push_back(faceIndices[2][i] + base_index);
                       }
                   }
                }
           }
       }
   }

    // Update VBO and EBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Chunk::draw() {
    glBindVertexArray(this->vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
