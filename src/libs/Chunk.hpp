
#ifndef CHUNK_HPP
#define CHUNK_HPP
#include <vector>
#include <glm/ext/vector_float3.hpp>




class Chunk {
private:
    unsigned int vao, vbo, ebo;
    std::vector<float> vertices;
    std::vector<int> indices;

    void gen_mesh(const std::vector<glm::vec3>& positions); // New method for meshing

public:
    Chunk();
    void gen_chunk_data(const std::vector<glm::vec3>& data);
    void draw();
};

#endif // CHUNK_HPP
