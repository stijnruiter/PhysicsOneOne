#include "IndexBuffer.hpp"

#include "gl_debug.hpp"

IndexBuffer::IndexBuffer(const unsigned int* indices, const int count, GLenum usage)
{
    this->count = count;
    GLCHECK(glGenBuffers(1, &bufferId));
    GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId));
    GLCHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), indices, usage));

}

IndexBuffer::~IndexBuffer()
{
    GLCHECK(glDeleteBuffers(1, &bufferId));
    bufferId = 0;
    count = 0;
}

void IndexBuffer::Bind() const
{
    GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId));
}

void IndexBuffer::Unbind() const
{
    GLCHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

unsigned int IndexBuffer::GetCount() const
{
    return this->count;
}
