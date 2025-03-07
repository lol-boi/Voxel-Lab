#ifndef SHADER_H
#define SHADER_H
#include <string>
#include "../external/glm/ext/matrix_float4x4.hpp"

class Shader {
    public:
        unsigned int ID;

        Shader(const char* vertex_path, const char* fragment_path);
        ~Shader();

        void use() const;
        void set_mat4(const std::string &name, const glm::mat4 &mat) const;
        void set_bool(const std::string &name, bool value) const;
        void set_int(const std::string &name, int value) const;
        void set_float(const std::string &name, float value) const;

    private:
        std::string read_file_to_string(const char* file_path);
};

#endif
