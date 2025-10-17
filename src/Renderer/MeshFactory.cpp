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
} // namespace nre
