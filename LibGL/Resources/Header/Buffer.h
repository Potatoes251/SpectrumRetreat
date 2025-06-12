#pragma once
#include <glad/glad.h>

class Buffer 
{
public:
    GLuint  m_id;
    GLenum  m_type; // GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER...

    Buffer();
    ~Buffer();

    void    init(GLenum type); // GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, etc.
    void    setData(const void* data, GLsizeiptr size, GLenum usage);
    void    bind() const;
    void    unbind() const;
    void    destroy();
};
