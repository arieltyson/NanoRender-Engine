#include "Platform/OpenGL/GLShader.h"

#if defined(NRE_ENABLE_OPENGL) && defined(NRE_USE_GLFW)

#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

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
GLShader::GLShader(std::vector<ShaderSource> sources) : sources_(std::move(sources)) {}

GLShader::~GLShader()
{
    destroyProgram();
}

void GLShader::compile()
{
    destroyProgram();

    program_ = glCreateProgram();
    if (program_ == 0)
    {
        throw std::runtime_error("Failed to create OpenGL shader program.");
    }

    std::vector<unsigned int> compiledStages;
    compiledStages.reserve(sources_.size());

    try
    {
        for (const auto& source : sources_)
        {
            const unsigned int shader = compileStage(source);
            glAttachShader(program_, shader);
            compiledStages.push_back(shader);
        }

        glLinkProgram(program_);

        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program_, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE)
        {
            GLint logLength = 0;
            glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &logLength);
            std::string infoLog(static_cast<std::size_t>(logLength), '\0');
            glGetProgramInfoLog(program_, logLength, nullptr, infoLog.data());
            throw std::runtime_error("Failed to link OpenGL shader program: " + infoLog);
        }
    }
    catch (...)
    {
        for (const auto shader : compiledStages)
        {
            glDetachShader(program_, shader);
            glDeleteShader(shader);
        }
        destroyProgram();
        throw;
    }

    for (const auto shader : compiledStages)
    {
        glDetachShader(program_, shader);
        glDeleteShader(shader);
    }
}

void GLShader::reload(const std::vector<ShaderSource>& sources)
{
    sources_ = sources;
    compile();
}

void GLShader::bind() const
{
    glUseProgram(program_);
}

void GLShader::unbind() const
{
    glUseProgram(0);
}

void GLShader::setMatrix4(std::string_view name, const float* data)
{
    if (program_ == 0)
    {
        throw std::runtime_error("Attempted to set uniform on an uninitialized shader program.");
    }

    const int location = uniformLocation(name);
    if (location >= 0)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, data);
    }
}

void GLShader::bindUniformBlock(std::string_view name, unsigned int binding)
{
    if (program_ == 0)
    {
        throw std::runtime_error("Attempted to bind uniform block on an uninitialized shader program.");
    }

    const std::string key{name};
    const GLuint blockIndex = glGetUniformBlockIndex(program_, key.c_str());
    if (blockIndex != GL_INVALID_INDEX)
    {
        glUniformBlockBinding(program_, blockIndex, binding);
    }
}

unsigned int GLShader::compileStage(const ShaderSource& source) const
{
    GLenum shaderType = 0;
    switch (source.stage)
    {
    case ShaderStage::Vertex:
        shaderType = GL_VERTEX_SHADER;
        break;
    case ShaderStage::Fragment:
        shaderType = GL_FRAGMENT_SHADER;
        break;
    case ShaderStage::Compute:
#if defined(GL_COMPUTE_SHADER)
        shaderType = GL_COMPUTE_SHADER;
        break;
#else
        throw std::runtime_error("OpenGL compute shaders are not supported on this platform.");
#endif
    }

    const unsigned int shader = glCreateShader(shaderType);
    if (shader == 0)
    {
        throw std::runtime_error("Failed to create OpenGL shader stage.");
    }

    const char* sourcePtr = source.source.c_str();
    const GLint sourceLength = static_cast<GLint>(source.source.size());
    glShaderSource(shader, 1, &sourcePtr, &sourceLength);
    glCompileShader(shader);

    GLint compileStatus = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus != GL_TRUE)
    {
        GLint logLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::string infoLog(static_cast<std::size_t>(logLength), '\0');
        glGetShaderInfoLog(shader, logLength, nullptr, infoLog.data());
        glDeleteShader(shader);
        throw std::runtime_error("Failed to compile OpenGL shader stage: " + infoLog);
    }

    return shader;
}

void GLShader::destroyProgram() noexcept
{
    if (program_ != 0)
    {
        glDeleteProgram(program_);
        program_ = 0;
        uniformLocationCache_.clear();
    }
}

int GLShader::uniformLocation(std::string_view name)
{
    const std::string key{name};
    if (const auto it = uniformLocationCache_.find(key); it != uniformLocationCache_.end())
    {
        return it->second;
    }

    const int location = glGetUniformLocation(program_, key.c_str());
    uniformLocationCache_.emplace(key, location);
    return location;
}
} // namespace nre

#endif // NRE_ENABLE_OPENGL && NRE_USE_GLFW
