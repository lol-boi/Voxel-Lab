#ifndef CHUNK_HPP
#define CHUNK_HPP
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float3.hpp>




class Chunk {

    public:
        glm::vec3 chunk_pos_in_world;
        bool chunk_data_present;
        unsigned int instance_count;
        int gpu_mem_offset;

        Chunk(glm::vec3);
        ~Chunk();

        int get_height(int, int);
        void update_mesh();
        void gen_chunk_data(int seed);
        void cull_face(int *);

    private:
        int *chunk_data;
        const static int c_size = 32;
        const static int c_size_p = 34;

        int pack_data(int,int,int,int,int);
        int pack_greedy_quads(int,int,int,int,int,int,int);
        void greedy_mesh(unsigned *,int,int,int *);
};

#endif // CHUNK_HPP
