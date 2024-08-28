
#ifndef CHUNK_HPP
#define CHUNK_HPP
#include <glm/detail/qualifier.hpp>
#include <vector>
#include <glm/ext/vector_float3.hpp>




class Chunk {
private:
    int *chunk_data;
    int pack_data(int,int,int,int,int);

public:

    glm::vec3 chunk_pos_in_world;
    int get_height(int, int);
    Chunk(glm::vec3);
    void gen_mesh(int, std::vector<int> *);
    void update_mesh();
    void gen_chunk_data(int chunk_size, int seed);
};

#endif // CHUNK_HPP
