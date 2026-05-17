#pragma once

#include <glad/glad.h>
#include <string>
#include <iostream>
#include <filesystem>

//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

class GLTexture
{
public:
    GLuint id = 0;
    int width = 0, height = 0, nrChannels = 0;

    GLTexture() = default;

    GLTexture(const std::filesystem::path& filename)
    {
        load(filename.string());
    }

    bool load(const std::string& filename)
    {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        // Wrapping & filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // Mipmap
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //// Optional: anisotropic filtering
        //if (GLAD_GL_EXT_texture_filter_anisotropic) {
        //    GLfloat maxAniso = 0.0f;
        //    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
        //    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);
        //}

        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

        if (data) 
        {
            GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            stbi_image_free(data);

            std::cout << "Loaded texture: " << filename << " (" << width << "x" << height << ")\n";

            return true;
        }
        else 
        {
            std::cerr << "Failed to load texture: " << filename << std::endl;
            stbi_image_free(data);

            return false;
        }
    }

    void bind(GLuint textureUnit = 0) const
    {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, id);
    }

    ~GLTexture()
    {
        if (id != 0) {
            glDeleteTextures(1, &id);
        }
    }
};