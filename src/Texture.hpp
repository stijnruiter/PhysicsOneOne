#pragma once

#include <string>
#include <glad/glad.h>

class Texture
{
private:
	int width = 0;
	int height = 0;
	int nrChannels = 0;

	unsigned int textureId = 0;
	GLenum textureSlot;
public:
	Texture(const std::string& imageFilePath, const GLenum slot = GL_TEXTURE0);
	~Texture();

	void SetWrap(GLenum wrapOptionS, GLenum wrapOptionT) const;
	void SetMinMag(GLenum minOption, GLenum magOption) const;
	void SetParameter(GLenum name, GLenum value) const;

	void Bind() const;
	void Unbind() const;
};