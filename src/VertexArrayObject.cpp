#include "VertexArrayObject.hpp"

#include "gl_debug.hpp"
#include "Logger.hpp"

VertexArrayObject::VertexArrayObject()
{
    GLCHECK(glGenVertexArrays(1, &vertexArrayId));
    GLCHECK(glBindVertexArray(vertexArrayId));
}

VertexArrayObject::~VertexArrayObject()
{
    GLCHECK(glDeleteVertexArrays(1, &vertexArrayId));
    vertexArrayId = 0;
}

void VertexArrayObject::Bind() const
{
    GLCHECK(glBindVertexArray(vertexArrayId));
}

void VertexArrayObject::Unbind() const
{
    GLCHECK(glBindVertexArray(0));
}

void VertexArrayObject::AddBuffer(const VertexBuffer& buffer)
{
    Bind();
    buffer.Bind();

    const auto& attributes = buffer.GetAttributes();
    short offset = 0;

    for (size_t i = 0; i < attributes.size(); i++)
    {
        const auto& attribute = attributes[i];

        if (attribute.Location < 0)
        {
            Engine::Logger::LogWarning("Shader location in VertexBuffer not valid.");
            offset += attribute.Size; // invalid location, dont set the vertex attrib, but still compute the offset for other data in the buffer
            continue;
        }

        GLint enabled;
        GLCHECK(glGetVertexAttribiv(attribute.Location, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled));
        if (enabled == GL_TRUE)
        {
            Engine::Logger::LogWarning("Shader location {0} was already enabled in this VertexArrayObject.\n"
                "This location will now be overwritten. Are you sure this is correct ? ", attribute.Location);
            DEBUG_BREAK();
        }

        GLCHECK(glVertexAttribPointer(attribute.Location, attribute.Count, attribute.Type, attribute.Normalized, buffer.GetStride(), (void*)offset));
        GLCHECK(glEnableVertexAttribArray(attribute.Location));

        offset += attribute.Size;
    }
}
