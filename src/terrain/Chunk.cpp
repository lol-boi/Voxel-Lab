#include "Chunk.hpp"
#include "FastNoiseLite.h"
#include <bits/types/clockid_t.h>
#include <glm/ext/vector_float3.hpp>
#include <cstdint>
#include <array>
#include <unordered_map>
#include <limits.h>

Chunk::Chunk(glm::vec3 pos) : chunk_data(nullptr) {
    chunk_pos_in_world = pos;
}

void Chunk::gen_chunk_data(int seed){

    delete [] chunk_data;
    chunk_data = new int[c_size_p*c_size_p*c_size_p]();

    float oct1 = .88f, oct2 = .5f, oct3 = .22f;
    float freq1 = .22f, freq2 = .55f, freq3  = .88f;

    FastNoiseLite noise(seed);
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    for(int x = 0; x<c_size_p; x++){
        for(int z = 0; z<c_size_p;  z++){

           int a,b;
           a = (chunk_pos_in_world.x * c_size) + x - 1;
           b = (chunk_pos_in_world.z * c_size) + z - 1;

            float height =
                  oct1 * noise.GetNoise(freq1*(float)a,freq1*(float)b) +
                    oct2 * noise.GetNoise(freq2*(float)a,freq2*(float)b) +
                    oct3 * noise.GetNoise(freq3*(float)a,freq3*(float)b);

             height = height /(oct1 + oct2 + oct3);
             height = (int) ((height+1) * 256/2);
            if(height < 60) height  = 60;

            for(int y = 0; y<c_size_p; y++){
                int world_y = (c_size * chunk_pos_in_world.y) + y;
                int index = (c_size_p * c_size_p * y) + (c_size_p * z) + x;
                //int index = (c_size * c_size * (y-(c_size * chunk_pos_in_world.y))) + (c_size * z) + x;
                if (height >= world_y) {
                    if (world_y == 0) {
                        chunk_data[index] = 0;
                    } else if(world_y == 1){
                        chunk_data[index] = 4; //
                    } else if (world_y == height) {
                        chunk_data[index] = 1; // grass
                    } else if (world_y < height && world_y >= height - 4) {
                        chunk_data[index] = 3; // dirt
                    } else if (world_y < height - 4) {
                        chunk_data[index] = 2; // stone
                    }
                } else {
                    chunk_data[index] = 0; // air
                }
            }
        }
    }
}

Chunk::~Chunk(){
    if(chunk_data){
        delete []chunk_data;
        chunk_data = nullptr;
    }
}

void Chunk::cull_face(int* instance_data){
    instance_count  = 0;
    int c_size2 = c_size*c_size;

    uint64_t* x_chunk = new uint64_t[c_size_p * c_size_p](); // coords y,z
    uint64_t* y_chunk = new uint64_t[c_size_p * c_size_p](); // coords x,z
    uint64_t* z_chunk = new uint64_t[c_size_p * c_size_p](); // coords x,y


    // Iterate over the chunk_data
    for (int y = 0; y < c_size_p; ++y) {
        for (int z = 0; z < c_size_p; ++z) {
            for (int x = 0; x < c_size_p; ++x) {
                int index = (c_size_p * c_size_p * y) + (c_size_p * z) + x; // Calculate the index

                int blockType = chunk_data[index]; // Assuming blockType is 0 for air and 1 for solid

                // Check if the block is solid or air
                if (blockType != 0) {
                    x_chunk[y * c_size_p + z] |= (1LL << x); // Set the bit in x_chunk for y,z plane
                    y_chunk[x * c_size_p + z] |= (1LL << y); // Set the bit in y_chunk for x,z plane
                    z_chunk[y * c_size_p + x] |= (1LL << z); // Set the bit in z_chunk for x,y plane
                }
            }
        }
    }


    uint32_t *culled_x = new uint32_t[2 * c_size * c_size]; //culled_x[2][y][z] --> culled_x[ 2 * y * c_size_p + z];
    uint32_t *culled_y = new uint32_t[2 * c_size * c_size]; //culled_y[2][x][z] --> culled_y[ 2 * x * c_size_p + z];
    uint32_t *culled_z = new uint32_t[2 * c_size * c_size]; //culled_z[2][x][y] --> culled_z[ 2 * y * c_size_p + x];

    for(int u = 0; u<c_size ; u++){
        for(int v = 0; v<c_size; ++v){

            int i = ((u+1) * c_size_p ) + v+1;
            int j = ((u*c_size)) + (v);

            {//cull face along +axis
            culled_x[j] = static_cast<uint32_t>((( ~(x_chunk[i] << 1) & x_chunk[i])>>1) & 0xFFFFFFFF); //cull left faces
            culled_y[j] = static_cast<uint32_t>((( ~(y_chunk[i] << 1) & y_chunk[i])>>1) & 0xFFFFFFFF); //cull down faces
            culled_z[j] = static_cast<uint32_t>((( ~(z_chunk[i] << 1) & z_chunk[i])>>1) & 0xFFFFFFFF); // cull forward faces
            }

            {//cull face along -axis
            culled_x[j+(c_size2)]= static_cast<uint32_t>(((x_chunk[i] & ~(x_chunk[i] >> 1))>>1) & 0xFFFFFFFF); //cull right faces
            culled_y[j+(c_size2)]= static_cast<uint32_t>(((y_chunk[i] & ~(y_chunk[i] >> 1))>>1) & 0xFFFFFFFF); //cull top faces
            culled_z[j+(c_size2)]= static_cast<uint32_t>(((z_chunk[i] & ~(z_chunk[i] >> 1))>>1) & 0xFFFFFFFF); // cull back faceys
            }
        }
    }

    delete [] x_chunk; x_chunk = nullptr;
    delete [] y_chunk; y_chunk = nullptr;
    delete [] z_chunk; z_chunk = nullptr;


    std::unordered_map<int, std::array<unsigned int, 32>> arr[6]; //using int here could use a smaller data type cuse we are only storing pos (0-31) and block type here;

    for(int face = 0; face<=5; face++){
        int axis = face / 2;
        uint32_t *chunk;
        if(axis == 0){
            chunk = culled_z; //for pane xy
        }
        else if(axis == 1){
            chunk = culled_x;//for palane zy
        }
        else{
           chunk = culled_y;//for pane xz
        }
        for(int col = 0; col<c_size; col++){
            for(int row = 0; row<c_size; row++){
                int val = chunk[((face%2) * c_size2) + (col*c_size + row)];
                while(val != 0){
                    int bit = __builtin_ctz(val);
                    val = val & (val - 1);
                    int tex;
                    int key;
                    switch(axis){
                        case 0:
                            tex = chunk_data[(c_size_p * c_size_p * (col+1) )+ (c_size_p * (bit+1)) + (row+1)];
                            key = (bit << 4) | tex;
                            arr[face][key][row] |= (1 << col);
                            break;
                        case 1:
                            tex = chunk_data[(c_size_p * c_size_p * (col+1) )+ (c_size_p * (row+1)) + (bit+1)];
                            key = (bit << 4) | tex;
                            arr[face][key][row] |= (1 << col);
                            //std::cout << "Inserted at (" << bit << ","<< col << ","<< row <<")" <<std::endl;
                            break;
                        case 2:
                            tex = chunk_data[(c_size_p * c_size_p * (bit+1) )+ (c_size_p * (row+1)) + (col+1)];
                            key = (bit << 4) | tex;
                            arr[face][key][row] |= (1 << col);
                            break;
                    }
                }
            }
        }
    }

    delete []culled_x; culled_x = nullptr;
    delete []culled_y; culled_y = nullptr;
    delete []culled_z; culled_z = nullptr;

    for(int face = 0; face<=5; face++){
        for(auto i : arr[face]){
            greedy_mesh(i.second.data(),face,i.first, instance_data);
        }
    }

}



int Chunk::pack_greedy_quads(int x, int y, int z, int normal, int texture, int height, int width){
    int data = 0;
    data |= (x & 31);               // 5 bits for x (0-31)
    data |= (y & 31) << 5;          // 5 bits for y (0-31)
    data |= (z & 31) << 10;         // 5 bits for z (0-31)
    data |= (normal & 7) << 15;     // 3 bits for normal (0-7) //should be updated when the normal will be send via ssbo
    data |= (texture & 15) << 18;   // 4 bits for texture index (0-15)
    data |= (height & 31) << 22;    // 5 bits for height of quad
    data |= (width & 31) << 27;     // 5 bits for width of quad
    return data;
}

void Chunk::greedy_mesh(unsigned int* data, int dir, int key,int* instance_data){
    //greedy meshing implimentation
    for(int row = 0; row<c_size; row++){
        int y = 0; //at what bit index are we at
        while(y < c_size){
            y += __builtin_ctz(data[row] >> y);
            if(y >= c_size) continue;
            int height = __builtin_ctz( ~(data[row] >> y));
            int h_mask;
            if(height == c_size) h_mask = INT_MAX;
            else h_mask = ((1u << height) - 1);
            int mask = h_mask << y;
            int width = 1;
            while(row+width < c_size){
                int next_row_h = (data[row+width] >> y) & h_mask;
                if(next_row_h != h_mask){
                    break;
                }
                data[row+width] = data[row+width ] & ~mask;
                width += 1;
            }
            int tex = key & 0xF;
            int z = key >> 4;
            int packed_data  =0;
            if(dir /2 == 0){
                packed_data = (pack_greedy_quads(row, y, z, dir, tex, height-1, width-1));
            }
            else if(dir / 2 == 1){
                packed_data = (pack_greedy_quads(z, y, row, dir, tex, height-1, width-1));
            }
            else{
                packed_data = (pack_greedy_quads(y, z, row, dir, tex, height-1, width-1));
            }
            instance_data[instance_count++] = packed_data;
            y += height;
        }
    }
}
