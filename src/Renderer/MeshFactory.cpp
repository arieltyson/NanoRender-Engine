#include "Renderer/MeshFactory.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#include <tiny_gltf.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace nre
{
namespace
{
std::string toLower(std::string value)
{
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });
    return value;
}

Vertex makeVertexFromObj(const tinyobj::attrib_t& attrib, const tinyobj::index_t& idx)
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

MeshData loadObjMesh(const std::string& path)
{
    tinyobj::ObjReaderConfig config;
    config.triangulate = true;
    config.mtl_search_path = std::filesystem::path(path).parent_path().string();

    tinyobj::ObjReader reader;
    if (!reader.ParseFromFile(path, config))
    {
        throw std::runtime_error("Failed to load OBJ: " + path + "\n" + reader.Error());
    }

    const auto& attrib = reader.GetAttrib();
    const auto& shapes = reader.GetShapes();

    if (attrib.vertices.empty())
    {
        throw std::runtime_error("OBJ has no vertex positions: " + path);
    }

    MeshData data{};
    for (const auto& shape : shapes)
    {
        for (const auto& idx : shape.mesh.indices)
        {
            data.vertices.push_back(makeVertexFromObj(attrib, idx));
            data.indices.push_back(static_cast<std::uint32_t>(data.vertices.size() - 1));
        }
    }

    if (data.indices.empty())
    {
        throw std::runtime_error("OBJ contained no faces: " + path);
    }

    return data;
}

std::size_t componentSize(int componentType)
{
    return static_cast<std::size_t>(tinygltf::GetComponentSizeInBytes(componentType));
}

std::size_t componentCount(int type)
{
    return static_cast<std::size_t>(tinygltf::GetNumComponentsInType(type));
}

const tinygltf::BufferView& bufferView(const tinygltf::Model& model, const tinygltf::Accessor& accessor)
{
    if (accessor.bufferView < 0)
    {
        throw std::runtime_error("GLTF accessor has no bufferView");
    }
    return model.bufferViews.at(static_cast<std::size_t>(accessor.bufferView));
}

const unsigned char* accessorData(const tinygltf::Model& model,
                                  const tinygltf::Accessor& accessor,
                                  std::size_t index)
{
    const auto& view = bufferView(model, accessor);
    const auto& buffer = model.buffers.at(static_cast<std::size_t>(view.buffer));
    const std::size_t stride = accessor.ByteStride(view) == 0
                                   ? componentSize(accessor.componentType) * componentCount(accessor.type)
                                   : static_cast<std::size_t>(accessor.ByteStride(view));
    return buffer.data.data() + view.byteOffset + accessor.byteOffset + stride * index;
}

MeshData loadGltfMesh(const std::string& path)
{
    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string error;
    std::string warning;

    const std::string ext = toLower(std::filesystem::path(path).extension().string());
    bool success = false;
    if (ext == ".glb")
    {
        success = loader.LoadBinaryFromFile(&model, &error, &warning, path);
    }
    else
    {
        success = loader.LoadASCIIFromFile(&model, &error, &warning, path);
    }

    if (!warning.empty())
    {
        std::cerr << "GLTF warning: " << warning << '\n';
    }
    if (!success)
    {
        throw std::runtime_error("Failed to load GLTF: " + path + "\n" + error);
    }

    if (model.meshes.empty())
    {
        throw std::runtime_error("GLTF contains no meshes: " + path);
    }

    MeshData data{};

    for (const auto& mesh : model.meshes)
    {
        for (const auto& primitive : mesh.primitives)
        {
            if (primitive.mode != TINYGLTF_MODE_TRIANGLES)
            {
                continue;
            }

            const auto positionIt = primitive.attributes.find("POSITION");
            if (positionIt == primitive.attributes.end())
            {
                continue;
            }

            const auto& positionAccessor = model.accessors[positionIt->second];
            if (positionAccessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT ||
                positionAccessor.type != TINYGLTF_TYPE_VEC3)
            {
                throw std::runtime_error("GLTF mesh POSITION must be VEC3 float");
            }

            std::vector<Vertex> vertices(positionAccessor.count);

            for (std::size_t i = 0; i < positionAccessor.count; ++i)
            {
                const float* values = reinterpret_cast<const float*>(accessorData(model, positionAccessor, i));
                vertices[i].position[0] = values[0];
                vertices[i].position[1] = values[1];
                vertices[i].position[2] = values[2];
            }

            const auto normalIt = primitive.attributes.find("NORMAL");
            if (normalIt != primitive.attributes.end())
            {
                const auto& normalAccessor = model.accessors[normalIt->second];
                if (normalAccessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT ||
                    normalAccessor.type != TINYGLTF_TYPE_VEC3)
                {
                    throw std::runtime_error("GLTF mesh NORMAL must be VEC3 float");
                }

                for (std::size_t i = 0; i < vertices.size(); ++i)
                {
                    const float* values = reinterpret_cast<const float*>(accessorData(model, normalAccessor, i));
                    vertices[i].normal[0] = values[0];
                    vertices[i].normal[1] = values[1];
                    vertices[i].normal[2] = values[2];
                }
            }
            else
            {
                for (auto& vertex : vertices)
                {
                    vertex.normal[0] = 0.0F;
                    vertex.normal[1] = 0.0F;
                    vertex.normal[2] = 1.0F;
                }
            }

            const auto texIt = primitive.attributes.find("TEXCOORD_0");
            if (texIt != primitive.attributes.end())
            {
                const auto& texAccessor = model.accessors[texIt->second];
                if (texAccessor.componentType != TINYGLTF_COMPONENT_TYPE_FLOAT ||
                    texAccessor.type != TINYGLTF_TYPE_VEC2)
                {
                    throw std::runtime_error("GLTF mesh TEXCOORD_0 must be VEC2 float");
                }

                for (std::size_t i = 0; i < vertices.size(); ++i)
                {
                    const float* values = reinterpret_cast<const float*>(accessorData(model, texAccessor, i));
                    vertices[i].uv[0] = values[0];
                    vertices[i].uv[1] = values[1];
                }
            }

            const std::size_t baseVertex = data.vertices.size();
            data.vertices.insert(data.vertices.end(), vertices.begin(), vertices.end());

            if (primitive.indices >= 0)
            {
                const auto& indexAccessor = model.accessors[primitive.indices];
                const std::size_t indexCount = indexAccessor.count;

                for (std::size_t i = 0; i < indexCount; ++i)
                {
                    std::uint32_t indexValue = 0;
                    const unsigned char* ptr = accessorData(model, indexAccessor, i);
                    switch (indexAccessor.componentType)
                    {
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
                    {
                        std::uint8_t value = 0;
                        std::memcpy(&value, ptr, sizeof(value));
                        indexValue = value;
                        break;
                    }
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
                    {
                        std::uint16_t value = 0;
                        std::memcpy(&value, ptr, sizeof(value));
                        indexValue = value;
                        break;
                    }
                    case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
                    {
                        std::uint32_t value = 0;
                        std::memcpy(&value, ptr, sizeof(value));
                        indexValue = value;
                        break;
                    }
                    default:
                        throw std::runtime_error("Unsupported GLTF index component type");
                    }
                    data.indices.push_back(static_cast<std::uint32_t>(baseVertex + indexValue));
                }
            }
            else
            {
                for (std::size_t i = 0; i < vertices.size(); ++i)
                {
                    data.indices.push_back(static_cast<std::uint32_t>(baseVertex + i));
                }
            }
        }
    }

    if (data.indices.empty())
    {
        throw std::runtime_error("GLTF mesh has no triangle data: " + path);
    }

    return data;
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
    const std::string ext = toLower(std::filesystem::path(path).extension().string());
    if (ext == ".obj")
    {
        return loadObjMesh(path);
    }
    if (ext == ".gltf" || ext == ".glb")
    {
        return loadGltfMesh(path);
    }

    throw std::runtime_error("Unsupported mesh format: " + path);
}

MeshData addIndexedMesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices)
{
    MeshData data{};
    data.vertices = vertices;
    data.indices = indices;
    return data;
}
} // namespace nre
