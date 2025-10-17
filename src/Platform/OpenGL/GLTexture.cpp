#include "Platform/OpenGL/GLTexture.h"

#if defined(NRE_ENABLE_OPENGL) && defined(NRE_USE_GLFW)

#include <stdexcept>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl3.h>
#else
#include <GL/gl.h>
#endif

namespace nre
{
namespace
{
GLenum toGLInternalFormat(TextureFormat format)
{
    switch (format)
    {
    case TextureFormat::RGBA8:
        return GL_RGBA8;
    case TextureFormat::RGBA16F:
        return GL_RGBA16F;
    case TextureFormat::Depth24Stencil8:
        return GL_DEPTH24_STENCIL8;
    }
    return GL_RGBA8;
}

GLenum toGLFormat(TextureFormat format)
{
    switch (format)
    {
    case TextureFormat::RGBA8:
    case TextureFormat::RGBA16F:
        return GL_RGBA;
    case TextureFormat::Depth24Stencil8:
        return GL_DEPTH_STENCIL;
    }
    return GL_RGBA;
}

GLenum toGLType(TextureFormat format)
{
    switch (format)
    {
    case TextureFormat::RGBA8:
        return GL_UNSIGNED_BYTE;
    case TextureFormat::RGBA16F:
        return GL_HALF_FLOAT;
    case TextureFormat::Depth24Stencil8:
        return GL_UNSIGNED_INT_24_8;
    }
    return GL_UNSIGNED_BYTE;
}
} // namespace

GLTexture::GLTexture(const TextureDescriptor& descriptor) : descriptor_(descriptor) {}

GLTexture::~GLTexture()
{
    destroy();
}

void GLTexture::loadFromFile(const std::string& /*path*/)
{
    throw std::runtime_error("Use TextureLoader for managed texture loading.");
}

void GLTexture::upload(const void* data, std::size_t /*size*/)
{
    ensureCreated();
    glBindTexture(GL_TEXTURE_2D, textureId_);

    const GLenum internalFormat = toGLInternalFormat(descriptor_.format);
    const GLenum format = toGLFormat(descriptor_.format);
    const GLenum type = toGLType(descriptor_.format);

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 static_cast<GLint>(internalFormat),
                 static_cast<GLsizei>(descriptor_.width),
                 static_cast<GLsizei>(descriptor_.height),
                 0,
                 format,
                 type,
                 data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, descriptor_.generateMipmaps ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (descriptor_.generateMipmaps)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture::bind(std::uint32_t slot) const
{
    glActiveTexture(GL_TEXTURE0 + static_cast<GLenum>(slot));
    glBindTexture(GL_TEXTURE_2D, textureId_);
}

void GLTexture::ensureCreated()
{
    if (textureId_ == 0)
    {
        glGenTextures(1, &textureId_);
    }
}

void GLTexture::destroy() noexcept
{
    if (textureId_ != 0)
    {
        glDeleteTextures(1, &textureId_);
        textureId_ = 0;
    }
}
} // namespace nre

#endif // NRE_ENABLE_OPENGL && NRE_USE_GLFW
