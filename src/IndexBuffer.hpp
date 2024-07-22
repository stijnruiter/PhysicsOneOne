#pragma once

#include <glad\glad.h>

class IndexBuffer
{
private:
	unsigned int bufferId;
	unsigned int count;
public:
	IndexBuffer(const unsigned int* indices, const int count, GLenum usage = GL_STATIC_DRAW);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetCount() const;
};