#include "Renderer.hpp"
#include "../external/glad/include/glad/glad.h"
#include <cstring>
#include <iostream>

Render::Render(int max_chunks, int bucket_size1){
    vertices = {
        -0.5f, -0.5f, 0.5f,
         0.5f, -0.5f, 0.5f,
        -0.5f,  0.5f, 0.5f,
         0.5f,  0.5f, 0.5f
    };

    glGenBuffers(1,&indirect_buffer);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_buffer);

    glGenBuffers(1,&ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);

    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &gpu_mapped_ibo);

    glBindVertexArray(vao); //defines the attributes of vbo

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(float)*3, (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, gpu_mapped_ibo);
    int size = (max_chunks * bucket_size1);
    glBufferStorage(GL_ARRAY_BUFFER,size * sizeof(int), nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    instance_buffer_ptr = (int*)glMapBufferRange(GL_ARRAY_BUFFER, 0, size * sizeof(GLuint), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    if (!instance_buffer_ptr) {
        std::cerr << "Failed to map instance buffer!" << std::endl;
    }

    glVertexAttribIPointer(1,1,GL_UNSIGNED_INT,sizeof(GLuint),(void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribDivisor(1,1);

    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_buffer);
    glBufferStorage(GL_DRAW_INDIRECT_BUFFER, max_chunks * sizeof(DrawArraysIndirectCommand), nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    draw_command_ptr = (DrawArraysIndirectCommand*)glMapBufferRange(GL_DRAW_INDIRECT_BUFFER, 0, max_chunks * sizeof(DrawArraysIndirectCommand), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, max_chunks * sizeof(glm::vec4), nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    ssbo_ptr = (glm::vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, max_chunks * sizeof(glm::vec4), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);

    memset(ssbo_ptr, 0, max_chunks * sizeof(glm::vec4));

    glBindVertexArray(0);
}

void Render::draw(int max_chunks){

    glBindVertexArray(vao);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
    glMultiDrawArraysIndirect(GL_TRIANGLE_STRIP, nullptr, max_chunks, 0);
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << err << std::endl;
    }
}


Render::~Render() {
    if (instance_buffer_ptr) {
        glBindBuffer(GL_ARRAY_BUFFER, gpu_mapped_ibo);
        glUnmapBuffer(GL_ARRAY_BUFFER);
        instance_buffer_ptr = nullptr;
    }

    if (draw_command_ptr) {
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, indirect_buffer);
        glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);
        draw_command_ptr = nullptr;
    }

    if (ssbo_ptr) {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        ssbo_ptr = nullptr;
    }

    glDeleteBuffers(1, &indirect_buffer);
    glDeleteBuffers(1, &ssbo);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &gpu_mapped_ibo);
    glDeleteVertexArrays(1, &vao);
}
