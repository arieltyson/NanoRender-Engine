#include "Renderer/MeshFactory.h"

#include <filesystem>
#include <stdexcept>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace nre
{
namespace
{
Vertex makeVertex(const tinyobj::attrib_t& attrib, const tinyobj::index_t& idx)
{
    Vertex vertex{};

    if (idx.vertex_index >= 0)
    {
        const int vi = idx.vertex_index * 3;
        if (vi + 2 >= static_cast<int>(attrib.vertices.size()))
        {
            throw std::runtime_error("OBJ vertex index out of range");
        }
        vertex.position[0] = attrib.vertices[static_cast<std::size_t>(vi + 0)];
        vertex.position[1] = attrib.vertices[static_cast<std::size_t>(vi + 1)];
        vertex.position[2] = attrib.vertices[static_cast<std::size_t>(vi + 2)];
    }
    else
    {
        vertex.position[0] = 0.0F;
        vertex.position[1] = 0.0F;
        vertex.position[2] = 0.0F;
    }

    if (idx.normal_index >= 0)
    {
        const int ni = idx.normal_index * 3;
        if (ni + 2 >= static_cast<int>(attrib.normals.size()))
        {
            throw std::runtime_error("OBJ normal index out of range");
        }
        vertex.normal[0] = attrib.normals[static_cast<std::size_t>(ni + 0)];
        vertex.normal[1] = attrib.normals[static_cast<std::size_t>(ni + 1)];
        vertex.normal[2] = attrib.normals[static_cast<std::size_t>(ni + 2)];
    }
    else
    {
        vertex.normal[0] = 0.0F;
        vertex.normal[1] = 0.0F;
        vertex.normal[2] = 1.0F;
    }

    if (idx.texcoord_index >= 0)
    {
        const int ti = idx.texcoord_index * 2;
        if (ti + 1 >= static_cast<int>(attrib.texcoords.size()))
        {
            throw std::runtime_error("OBJ texcoord index out of range");
        }
        vertex.uv[0] = attrib.texcoords[static_cast<std::size_t>(ti + 0)];
        vertex.uv[1] = attrib.texcoords[static_cast<std::size_t>(ti + 1)];
    }
    else
    {
        vertex.uv[0] = 0.0F;
        vertex.uv[1] = 0.0F;
    }

    return vertex;
}
} // namespace

MeshData makeTriangle()
{
    MeshData data{};
    data.vertices = {
        {{-0.6F, -0.6F, 0.0F}, {0.0F, 0.0F, 1.0F}, {0.0F, 0.0F}},
        {{0.6F, -0.6F, 0.0F}, {0.0F, 0.0F, 1.0F}, {1.0F, 0.0F}},
        {{0.0F, 0.6F, 0.0F}, {0.0F, 0.0F, 1.0F}, {0.5F, 1.0F}},
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

MeshData loadMeshFromFile(const std::string& path)
{
    tinyobj::ObjReaderConfig config;
    config.triangulate = true;
    const std::filesystem::path filePath(path);
    config.mtl_search_path = filePath.parent_path().string();

    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(path, config))
    {
        throw std::runtime_error("Failed to load OBJ: " + path + "\n" + reader.Error());
    }
    if (!reader.Warning().empty())
    {
        // Optional: log warning via std::cerr or similar (left silent here to avoid dependencies).
    }

    const auto& attrib = reader.GetAttrib();
    const auto& shapes = reader.GetShapes();

    if (attrib.vertices.empty())
    {
        throw std::runtime_error("OBJ has no vertex positions: " + path);
    }

    MeshData data{};
    data.vertices.reserve(shapes.size() * 3);
    data.indices.reserve(shapes.size() * 3);

    for (const auto& shape : shapes)
    {
        for (const auto& idx : shape.mesh.indices)
        {
            data.vertices.push_back(makeVertex(attrib, idx));
            data.indices.push_back(static_cast<std::uint32_t>(data.vertices.size() - 1));
        }
    }

    if (data.indices.empty())
    {
        throw std::runtime_error("OBJ contained no faces: " + path);
    }

    return data;
}

MeshData addIndexedMesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices)
{
    MeshData data{};
    data.vertices = vertices;
    data.indices = indices;
    return data;
}
} // namespace nre
