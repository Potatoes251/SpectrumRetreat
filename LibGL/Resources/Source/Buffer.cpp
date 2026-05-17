#include "Buffer.h"

Buffer::Buffer()
    : m_id(0), m_type(GL_ARRAY_BUFFER)
{
}

Buffer::~Buffer()
{
    destroy();
}

void    Buffer::init(GLenum type)
{
    m_type = type;
    glGenBuffers(1, &m_id);
}

void    Buffer::setData(const void* data, GLsizeiptr size, GLenum usage)
{
    bind();
    glBufferData(m_type, size, data, usage);
    unbind();
}

void    Buffer::bind() const
{
    glBindBuffer(m_type, m_id);
}

void    Buffer::unbind() const
{
    glBindBuffer(m_type, 0);
}

void    Buffer::destroy()
{
    if (m_id != 0)
    {
        glDeleteBuffers(1, &m_id);
        m_id = 0;
    }
}