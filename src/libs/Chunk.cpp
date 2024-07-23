#include "Chunk.hpp"
#include "glad/include/glad/glad.h"
#include <glm/ext/vector_float3.hpp>
#include <vector>

Chunk::Chunk() {
    // Initialize vertices and indices

    float vertices[40] = {
        // positions         // texture coords
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // 0
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 1
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 2
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 3

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 4
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 5
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 6
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f  // 7
    };

    int indices[36] = {
       1,0,3,3,2,1,
       4,5,6,6,7,4,
       0,1,5,5,4,0,
       2,3,7,7,6,2,
       0,4,7,7,3,0,
       1,2,6,6,5,1
    };

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * (3 + 2), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenBuffers(1, &instance_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, instance_vbo);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *)0);
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    glBindVertexArray(0);
}



void Chunk::gen_chunk_data(std::vector<glm::vec3> data) {
    positions = data;
    glBindBuffer(GL_ARRAY_BUFFER, instance_vbo);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);
    // Ensure positions vector is empty before generating new data
}


void Chunk::draw() {
    glBindVertexArray(this->vao);
    glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, positions.size());
}