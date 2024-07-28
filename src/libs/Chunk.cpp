
#include "Chunk.hpp"
#include "glad/include/glad/glad.h"
#include <glm/ext/vector_float3.hpp>
#include <vector>
#include <unordered_set>

// Custom hash function for glm::vec3
namespace std {
    template <>
    struct hash<glm::vec3> {
        std::size_t operator()(const glm::vec3& v) const {
            return ((std::hash<float>()(v.x) ^ (std::hash<float>()(v.y) << 1)) >> 1) ^ (std::hash<float>()(v.z) << 1);
        }
    };
}

// Direction vectors for the six faces of a cube
const glm::vec3 faceDirections[] = {
    glm::vec3(-1,  0,  0), // Left
    glm::vec3( 1,  0,  0), // Right
    glm::vec3( 0, -1,  0), // Bottom
    glm::vec3( 0,  1,  0), // Top
    glm::vec3( 0,  0, -1), // Back
    glm::vec3( 0,  0,  1)  // Front
};

// Vertices for the six faces of a cube
const float faceVertices[6][20] = {
    // Left face
    { -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f },

    // Right face
    {  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  0.0f, 0.0f },

    // Bottom face
    { -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 1.0f },

    // Top face
    { -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f },

    // Back face
    { -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f },

    // Front face
    { -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f }
};

// Indices for the six faces of a cube
const int faceIndices[6][6] = {
    { 0, 1, 2, 2, 3, 0 }, // Left
    { 0, 1, 2, 2, 3, 0 }, // Right
    { 0, 1, 2, 2, 3, 0 }, // Bottom
    { 0, 1, 2, 2, 3, 0 }, // Top
    { 0, 1, 2, 2, 3, 0 }, // Back
    { 0, 1, 2, 2, 3, 0 }  // Front
};

Chunk::Chunk() {
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

void Chunk::gen_chunk_data(const std::vector<glm::vec3>& data) {
    gen_mesh(data);
}

void Chunk::gen_mesh(const std::vector<glm::vec3>& positions) {
    // Clear previous data
    vertices.clear();
    indices.clear();

    // Store the positions in a set for quick lookup
    std::unordered_set<glm::vec3> positionSet(positions.begin(), positions.end());

    // Generate mesh data with face culling
    for (const auto& pos : positions) {
        for (int face = 0; face < 6; ++face) {
            glm::vec3 neighborPos = pos + faceDirections[face];
            if (positionSet.find(neighborPos) == positionSet.end()) {
                // Add face vertices
                for (int i = 0; i < 20; i += 5) {
                    vertices.push_back(faceVertices[face][i] + pos.x);
                    vertices.push_back(faceVertices[face][i + 1] + pos.y);
                    vertices.push_back(faceVertices[face][i + 2] + pos.z);
                    vertices.push_back(faceVertices[face][i + 3]);
                    vertices.push_back(faceVertices[face][i + 4]);
                }

                int base_index = vertices.size() / 5 - 4;
                for (int i = 0; i < 6; ++i) {
                    indices.push_back(faceIndices[face][i] + base_index);
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
