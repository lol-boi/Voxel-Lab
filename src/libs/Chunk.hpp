
#ifndef CHUNK_HPP
#define CHUNK_HPP
#include <glm/detail/qualifier.hpp>
#include <vector>
#include <glm/ext/vector_float3.hpp>




class Chunk {
private:
    glm::vec3 chunk_pos_in_world;
    unsigned int vao, vbo, ebo;
    int *chunk_data;
    std::vector<float> vertices;
    std::vector<int> indices;
    void gen_mesh(int);

public:
    int get_height(int, int);
    Chunk(glm::vec3);
    //~Chunk();
    void update_mesh();
    void gen_chunk_data(int, int);
    void draw();
};

#endif // CHUNK_HPP
