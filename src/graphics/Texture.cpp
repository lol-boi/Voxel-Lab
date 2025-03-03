#include "Texture.hpp"
#include "stb_image.h"
#include <stdexcept>

Texture::Texture(const std::string& path, bool useRGBA) {
    glGenTextures(1, &m_id);
    load_from_file(path, useRGBA);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_id);
}

void Texture::bind(GLenum unit) const {
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::load_from_file(const std::string& path, bool useRGBA) {
    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    const int desired_channels = useRGBA ? 4 : 3;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, desired_channels);

    if(!data) {
        stbi_image_free(data);
        throw std::runtime_error("Failed to load texture: " + path);
    }

    try {
        GLenum format = useRGBA ? GL_RGBA : GL_RGB;

        glBindTexture(GL_TEXTURE_2D, m_id);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Upload texture data
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
                    format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);

    } catch(...) {
        stbi_image_free(data);
        throw;
    }

    stbi_image_free(data);
}
