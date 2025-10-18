#pragma once

#include <cstdint>
#include <vector>

#include "Renderer/Mesh.h"

namespace nre
{
struct MeshData
{
    std::vector<Vertex> vertices;
    std::vector<std::uint32_t> indices;
};

MeshData makeTriangle();
MeshData makeFullscreenQuad();
MeshData loadMeshFromFile(const std::string& path);
MeshData addIndexedMesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices);
}
