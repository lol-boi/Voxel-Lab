#include "Texture.hpp"
#include "stb_image.h"
#include <vector>
#include <filesystem>
#include <algorithm>
#include <iostream>

Texture::Texture(const std::string& path, bool useRGBA) {
    glGenTextures(1, &m_id);
    load_textures_from_files(path, useRGBA);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_id);
}

void Texture::bind(GLenum unit) const {
    glActiveTexture(unit);
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);
    std::cout << "Exit bind!" << std::endl;
}

void Texture::load_textures_from_files(const std::string& folder_path, bool useRGBA) {
    stbi_set_flip_vertically_on_load(true);
    int width = 0;
    int height = 0;
    int nrchannels;
    const int desired_channels = useRGBA ? 4 : 3;
    std::vector<unsigned char*> texture_data;

    std::vector<std::filesystem::path> file_paths;
    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        file_paths.push_back(entry.path());
    }

    std::sort(file_paths.begin(), file_paths.end());
    std::cout << "Count: " << file_paths.size()<< std::endl;
    bool first_image = true;
    for (const auto& texture_path : file_paths) {
        int current_width, current_height;
        unsigned char* data = stbi_load(texture_path.c_str(), &current_width, &current_height, &nrchannels, desired_channels);

        if (!data) {
            std::cerr << "Failed to load image: " << texture_path << std::endl;
            for (unsigned char* loadedData : texture_data) {
                stbi_image_free(loadedData);
            }
            return;
        }

        if (first_image) {
            width = current_width;
            height = current_height;
            first_image = false;
        } else if (current_width != width || current_height != height) {
            std::cerr << "Error: Images have different dimensions. First: "
                      << width << "x" << height << ", Current: "
                      << current_width << "x" << current_height << std::endl;
            for (unsigned char* loadedData : texture_data) {
                stbi_image_free(loadedData);
            }
            stbi_image_free(data);
            return;
        }

        texture_data.push_back(data);
    }

    std::cout << "Loaded " << texture_data.size() << " textures" << std::endl;
    // Replace DSA functions with traditional ones
    glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width, height, texture_data.size());

    for (unsigned int i = 0; i < texture_data.size(); i++) {
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1,
                        useRGBA ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, texture_data[i]);
        stbi_image_free(texture_data[i]);
    }

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
