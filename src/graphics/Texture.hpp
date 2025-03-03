#pragma once
#include "../external/glad/include/glad/glad.h" // Replace GL/gl.h with glad
#include <string>

class Texture {
public:
    Texture(const std::string& path, bool useRGBA);
    ~Texture();
    void bind(GLenum unit) const;

private:
    GLuint m_id;
    void load_from_file(const std::string& path, bool useRGBA);
};
