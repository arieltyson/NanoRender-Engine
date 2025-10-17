#pragma once

#include "Renderer/Mesh.h"

#if defined(NRE_ENABLE_OPENGL) && defined(NRE_USE_GLFW)

namespace nre
{
class GLMesh final : public Mesh
{
public:
    GLMesh();
    ~GLMesh() override;

    void upload(const std::vector<Vertex>& vertices,
                const std::vector<std::uint32_t>& indices) override;
    void draw() const override;

private:
    unsigned int vao_ = 0;
    unsigned int vbo_ = 0;
    unsigned int ebo_ = 0;
    std::uint32_t indexCount_ = 0;
};
} // namespace nre

#endif // NRE_ENABLE_OPENGL && NRE_USE_GLFW
