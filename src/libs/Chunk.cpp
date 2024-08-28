#include "Chunk.hpp"
#include "FastNoiseLite.h"
#include <bits/types/clockid_t.h>
#include <glm/ext/vector_float3.hpp>
#include <vector>

Chunk::Chunk(glm::vec3 pos) {
    chunk_pos_in_world = pos;
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
}

int Chunk::pack_data(int x, int y, int z, int normal, int texture) {
    int data = 0;
    data |= (x & 31);               // 5 bits for x (0-31)
    data |= (y & 31) << 5;          // 5 bits for y (0-31)
    data |= (z & 31) << 10;         // 5 bits for z (0-31)
    data |= (normal & 7) << 15;     // 3 bits for normal (0-7)
    data |= (texture & 15) << 18;   // 4 bits for texture index (0-15)
    return data;
}

void Chunk::gen_mesh(int c_size, std::vector<int>* instance_data) {
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


                   if(front){ //0
                        unsigned int face= pack_data(x,y,z,0,N);
                       instance_data->push_back(face);
                   }
                   if(back){ //1
                        unsigned int face= pack_data(x,y,z,1,N);
                       instance_data->push_back(face);
                   }
                    if(right){ //2
                        unsigned int face= pack_data(x,y,z,2,N);
                        instance_data->push_back(face);
                    }
                    if(left){ //3
                        unsigned int face= pack_data(x,y,z,3,N);
                        instance_data->push_back(face);
                   }
                   if(top){ //4
                       unsigned int face = pack_data(x,y,z,4,N);
                        instance_data->push_back(face);
                   }
                   if(bottom){ //5
                       unsigned int face = pack_data(x,y,z,5,N);
                        instance_data->push_back(face);
                   }
                }
           }
       }
   }
}
