#ifndef CHUNK_HPP
#define CHUNK_HPP
#include <glm/detail/qualifier.hpp>
#include <vector>
#include <glm/ext/vector_float3.hpp>




class Chunk {

public:

    glm::vec3 chunk_pos_in_world;
    int get_height(int, int);
    Chunk(glm::vec3);
    ~Chunk();
    void update_mesh();
    void gen_chunk_data(int seed);
    void cull_face(int *);
    bool chunk_data_present;
    unsigned int instance_count;
private:
    int *chunk_data;
    int pack_data(int,int,int,int,int);
    int pack_greedy_quads(int,int,int,int,int,int,int);
    const static int c_size = 32;
    const static int c_size_p = 34;
    void greedy_mesh(unsigned *,int,int,int *);


};

#endif // CHUNK_HPP
