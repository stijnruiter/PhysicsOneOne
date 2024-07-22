#include "VertexBuffer.hpp"

#include "gl_debug.hpp"

VertexBuffer::VertexBuffer(const void* data, const unsigned int size, const GLenum usage) 
    : stride(0)
{
    GLCHECK(glGenBuffers(1, &bufferId));
    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, bufferId));
    GLCHECK(glBufferData(GL_ARRAY_BUFFER, size, data, usage));
}

VertexBuffer::~VertexBuffer()
{
    GLCHECK(glDeleteBuffers(1, &bufferId));
    bufferId = 0;
}

void VertexBuffer::Bind() const
{
    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, bufferId));
}

void VertexBuffer::Unbind() const
{
    GLCHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::DefineFloatAttribute(const int shaderLocation, const unsigned int count)
{
	attributes.push_back(AttributeDefinition(shaderLocation, GL_FLOAT, count, count * sizeof(float), GL_FALSE));

    stride += count * sizeof(float);
}

std::vector<AttributeDefinition> VertexBuffer::GetAttributes() const
{
    return attributes;
}

unsigned int VertexBuffer::GetStride() const
{
    return stride;
}
