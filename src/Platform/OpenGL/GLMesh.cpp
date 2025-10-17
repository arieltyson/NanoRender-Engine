#include "Platform/OpenGL/GLMesh.h"

#if defined(NRE_ENABLE_OPENGL) && defined(NRE_USE_GLFW)

#include <cstddef>
#include <stdexcept>
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
GLMesh::GLMesh() = default;

GLMesh::~GLMesh()
{
    if (ebo_ != 0)
    {
        glDeleteBuffers(1, &ebo_);
    }
    if (vbo_ != 0)
    {
        glDeleteBuffers(1, &vbo_);
    }
    if (vao_ != 0)
    {
        glDeleteVertexArrays(1, &vao_);
    }
}

void GLMesh::upload(const std::vector<Vertex>& vertices,
                    const std::vector<std::uint32_t>& indices)
{
    if (vertices.empty() || indices.empty())
    {
        throw std::runtime_error("GLMesh upload requires non-empty vertex and index data.");
    }

    if (vao_ == 0)
    {
        glGenVertexArrays(1, &vao_);
    }
    if (vbo_ == 0)
    {
        glGenBuffers(1, &vbo_);
    }
    if (ebo_ == 0)
    {
        glGenBuffers(1, &ebo_);
    }

    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)),
                 vertices.data(),
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(indices.size() * sizeof(std::uint32_t)),
                 indices.data(),
                 GL_STATIC_DRAW);

    const GLsizei stride = static_cast<GLsizei>(sizeof(Vertex));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(sizeof(float) * 3));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<const void*>(sizeof(float) * 6));

    glBindVertexArray(0);

    indexCount_ = static_cast<std::uint32_t>(indices.size());
    vertexCount_ = static_cast<std::uint32_t>(vertices.size());
}

void GLMesh::draw() const
{
    if (vao_ == 0 || indexCount_ == 0)
    {
        return;
    }

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount_), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
} // namespace nre

#endif // NRE_ENABLE_OPENGL && NRE_USE_GLFW
