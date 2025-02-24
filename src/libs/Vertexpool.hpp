#ifndef  VERTEXPOOL_HPP
#define VERTEXPOOL_HPP
#include "glad/include/glad/glad.h"
#include <deque>

class Vertexpool{
    private:
    unsigned int ibo;
    int buckets_count;
    int vertices_per_bucket;
    std::deque<int *> free_bucket_queue;

    public:
    Vertexpool(int n, int k): buckets_count(n), vertices_per_bucket(k) {}
    void reserve(int *start);
};

void Vertexpool::reserve(int *start){
    glGenBuffers(1,&ibo);
    glBindBuffer(GL_ARRAY_BUFFER, ibo);

    glBufferStorage(GL_ARRAY_BUFFER, max_instances * sizeof(int), nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
        instance_buffer_ptr = (int*)glMapBufferRange(GL_ARRAY_BUFFER, 0, max_instances * sizeof(GLuint), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
        if (!instance_buffer_ptr) {
            std::cerr << "Failed to map instance buffer!" << std::endl;
        }
    for(int i = 0; i<buckets_count; i++){
        free_bucket_queue.push_back(start+i * vertices_per_bucket);
    }
};

#endif
