#include "VertexAttributes.h"
#include <iostream>
VertexAttributes::VertexAttributes()
    : m_id(0)
{
}

VertexAttributes::~VertexAttributes()
{
    destroy();
}

void VertexAttributes::init()
{
    std::cout << "Before VAO init, m_id = " << m_id << std::endl;
    glGenVertexArrays(1, &m_id);
    std::cout << "After VAO init, m_id = " << m_id << std::endl;
}

void VertexAttributes::bind() const
{
    std::cout << m_id << std::endl;
    glBindVertexArray(m_id);
}

void VertexAttributes::unbind() const
{
    glBindVertexArray(0);
}

void VertexAttributes::destroy()
{
    if (m_id != 0)
    {
        glDeleteVertexArrays(1, &m_id);
        m_id = 0;
    }
}

void VertexAttributes::enableAttribute(GLuint index)
{
    glEnableVertexAttribArray(index);
}

void VertexAttributes::setAttributePointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer)
{
    glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}