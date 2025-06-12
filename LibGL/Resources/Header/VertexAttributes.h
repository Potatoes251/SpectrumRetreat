#pragma once
#include <glad/glad.h>

class VertexAttributes 
{
public:
    GLuint m_id;

    VertexAttributes();
    ~VertexAttributes();

    void    init();
    void    bind() const;
    void    unbind() const;
    void    destroy();

    void    enableAttribute(GLuint index);
    void    setAttributePointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
};
