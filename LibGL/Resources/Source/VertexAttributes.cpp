#include "VertexAttributes.h"

VertexAttributes::VertexAttributes()
    : m_id(0)
{
}

VertexAttributes::~VertexAttributes()
{
    destroy();
}

void    VertexAttributes::init()
{
    glGenVertexArrays(1, &m_id);
}

void    VertexAttributes::bind() const
{
    glBindVertexArray(m_id);
}

void    VertexAttributes::unbind() const
{
    glBindVertexArray(0);
}

void    VertexAttributes::destroy()
{
    if (m_id != 0)
    {
        glDeleteVertexArrays(1, &m_id);
        m_id = 0;
    }
}

void    VertexAttributes::enableAttribute(GLuint index)
{
    glEnableVertexAttribArray(index);
}

void    VertexAttributes::setAttributePointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
{
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}