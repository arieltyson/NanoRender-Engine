#include "Renderer/MeshFactory.h"

namespace nre
{
MeshData makeTriangle()
{
    MeshData data{};
    data.vertices = {
        {{-0.6F, -0.6F, 0.0F}, {0.0F, 0.0F, 1.0F}, {0.0F, 0.0F}},
        {{0.0F, 0.6F, 0.0F}, {0.0F, 0.0F, 1.0F}, {0.5F, 1.0F}},
        {{0.6F, -0.6F, 0.0F}, {0.0F, 0.0F, 1.0F}, {1.0F, 0.0F}},
    };
    data.indices = {0U, 1U, 2U};
    return data;
}

MeshData makeFullscreenQuad()
{
    MeshData data{};
    data.vertices = {
        {{-1.0F, -1.0F, 0.0F}, {0.0F, 0.0F, 1.0F}, {0.0F, 0.0F}},
        {{1.0F, -1.0F, 0.0F}, {0.0F, 0.0F, 1.0F}, {1.0F, 0.0F}},
        {{1.0F, 1.0F, 0.0F}, {0.0F, 0.0F, 1.0F}, {1.0F, 1.0F}},
        {{-1.0F, 1.0F, 0.0F}, {0.0F, 0.0F, 1.0F}, {0.0F, 1.0F}},
    };
    data.indices = {0U, 1U, 2U, 2U, 3U, 0U};
    return data;
}

MeshData loadMeshFromFile(const std::string& /*path*/)
{
    // Placeholder until asset pipeline is connected (e.g., Assimp integration).
    return makeTriangle();
}
} // namespace nre
