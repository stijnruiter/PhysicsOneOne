#include "Texture.hpp"

#include <stb/stb_image.h>
#include "gl_debug.hpp"
#include "Logger.hpp"

Texture::Texture(const std::string& imageFilePath, const GLenum slot) : textureSlot(slot)
{
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(imageFilePath.c_str(), &width, &height, &nrChannels, 0);
    if (!data)
    {
        Engine::Logger::LogError("Failed to load texture '{0}'", imageFilePath);
        DEBUG_BREAK();
        return;
    }

    if (width % 4 != 0)
    {
        // in case the width is not a multiple of 4, just set it to 1
        // TODO: check how to properly handle this, set always to 1, or find max integer, or change images.
        GLCHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
    }
    
    GLCHECK(glGenTextures(1, &textureId));
    GLCHECK(glActiveTexture(textureSlot));
    GLCHECK(glBindTexture(GL_TEXTURE_2D, textureId));

    GLCHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
    GLCHECK(glGenerateMipmap(GL_TEXTURE_2D));

    stbi_image_free(data);
}

Texture::~Texture()
{
    glDeleteTextures(1, &textureId);
    textureId = 0;
}

void Texture::SetWrap(GLenum wrapOptionS, GLenum wrapOptionT) const
{
    Bind();
    GLCHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapOptionS));
    GLCHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapOptionT));
}

void Texture::SetMinMag(GLenum minOption, GLenum magOption) const
{
    Bind();
    GLCHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minOption));
    GLCHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magOption));

}

void Texture::SetParameter(GLenum name, GLenum value) const
{
    Bind();
    GLCHECK(glTexParameteri(GL_TEXTURE_2D, name, value));
}

void Texture::Bind() const
{
    GLCHECK(glActiveTexture(textureSlot));
    GLCHECK(glBindTexture(GL_TEXTURE_2D, textureId));
}

void Texture::Unbind() const
{
    GLCHECK(glActiveTexture(textureSlot));
    GLCHECK(glBindTexture(GL_TEXTURE_2D, 0));
}