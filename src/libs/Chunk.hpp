
#ifndef CHUNK_HPP
#define CHUNK_HPP
#include <glm/detail/qualifier.hpp>
#include <vector>
#include <glm/ext/vector_float3.hpp>




class Chunk {
private:
    unsigned int vao, vbo, ibo;
    int *chunk_data;
    std::vector<float> vertices;
    std::vector<int> instance_data;
    void gen_mesh(int);
    int pack_data(int,int,int,int,int);

public:

    glm::vec3 chunk_pos_in_world;
    int get_height(int, int);
    Chunk(glm::vec3);
    //~Chunk();
    void update_mesh();
    void gen_chunk_data(int chunk_size, int seed);
    void draw();
};

#endif // CHUNK_HPP
