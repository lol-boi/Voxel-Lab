#ifndef SHADER_H
#define SHADER_H
#include <string>

class Shader {
    public:
        unsigned int ID;

        Shader(const char* vertex_path, const char* fragment_path);
        ~Shader();

        void use();
        void set_bool(const std::string &name, bool value) const;
        void set_int(const std::string &name, int value) const;
        void set_float(const std::string &name, float value) const;

    private:
        std::string read_file_to_string(const char* file_path);
};

#endif
