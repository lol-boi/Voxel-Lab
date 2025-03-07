#ifndef  RENDERER_HPP
#define  RENDERER_HPP
#include "../external/glm/ext/vector_float4.hpp"
#include "../external/glad/include/glad/glad.h"

#include <vector>

struct DrawArraysIndirectCommand {
    GLuint count;         // Number of vertices to draw
    GLuint instanceCount; // Number of instances to draw
    GLuint first;         // Starting index in the vertex array
    GLuint baseInstance;
    bool operator==(const DrawArraysIndirectCommand& other) const {
        return count == other.count &&
               instanceCount == other.instanceCount &&
               first == other.first &&
               baseInstance == other.baseInstance;
    } // Starting index in the instance array
};

class Render{
    public:
        int * instance_buffer_ptr; //instace data aka packed data
        DrawArraysIndirectCommand *draw_command_ptr; // draw_commands :)
        glm::vec4 *ssbo_ptr;

        Render(int max_chunks, int bucket_size1);
        ~Render();

        void draw(int max_chunks); //draw buffers

    private:
        std::vector<float> vertices; //predefined voxel structure
        unsigned int vbo, vao, gpu_mapped_ibo; //vertex_buffer_obj vertex_attribute_obj instance/element_buffer_obj
        unsigned int indirect_buffer; //for draw commands :)
        unsigned int ssbo;
};


#endif
