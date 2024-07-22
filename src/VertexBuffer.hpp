#pragma once

#include <glad\glad.h>
#include <vector>

struct AttributeDefinition
{
	int Location;
	GLenum Type;
	unsigned int Count;
	unsigned int Size;
	bool Normalized;

	AttributeDefinition(const int Location, const GLenum& Type, const unsigned int Count, const unsigned int Size, const bool Normalized)
		: Location(Location), Type(Type), Count(Count), Size(Size), Normalized(Normalized) { }
};

class VertexBuffer
{
private:
	unsigned int bufferId;
	std::vector<AttributeDefinition> attributes;
	unsigned int stride;

public:
	VertexBuffer(const void* data, unsigned int size, GLenum usage = GL_STATIC_DRAW);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

	void DefineFloatAttribute(const int shaderLocation, const unsigned int count);

	std::vector<AttributeDefinition> GetAttributes() const;
	unsigned int GetStride() const;

};