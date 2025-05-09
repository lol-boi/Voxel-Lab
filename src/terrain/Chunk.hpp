#ifndef CHUNK_HPP
#define CHUNK_HPP
#include "../external/glm/ext/vector_float3.hpp"

class Chunk {

    public:
        glm::vec3 chunk_pos_in_world;
        unsigned int instance_count; //Passed to draw_command struct

        Chunk(glm::vec3);
        ~Chunk();

        void gen_chunk_data();
        void cull_face(int *);

    private:
        int *chunk_data;
        const static int c_size = 32;
        const static int c_size_p = 34;


        inline int pack_greedy_quads(int,int,int,int,int,int,int);
        void greedy_mesh(unsigned int* ,int,int,int* );
        float gen_noise_e(int,int);
        float gen_noise_m(int,int);
};

#endif // CHUNK_HPP
