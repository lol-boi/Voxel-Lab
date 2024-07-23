#ifndef CHUNK_HPP
#define CHUNK_HPP
#include <vector>
#include <glm/ext/vector_float3.hpp>


class Chunk {
private:
    unsigned int vao, vbo, ebo, instance_vbo;
    float vertices[40];
    int indices[36];
    std::vector<glm::vec3> positions;



public:
    Chunk();
    void gen_chunk_data(std::vector<glm::vec3> data);
    void draw();
};

#endif // CHUNK_HPP
