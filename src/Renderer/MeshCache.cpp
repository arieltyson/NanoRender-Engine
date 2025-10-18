#include "Renderer/MeshCache.h"

#include <iostream>
#include <stdexcept>

#include "Renderer/RenderAPI.h"

namespace nre
{
MeshCache::MeshCache(RenderAPI& api) : api_(&api) {}

std::shared_ptr<Mesh> MeshCache::loadFromFile(const std::string& path)
{
    if (auto it = cache_.find(path); it != cache_.end())
    {
        if (auto existing = it->second.lock())
        {
            return existing;
        }
    }

    try
    {
        const MeshData data = loadMeshFromFile(path);
        auto mesh = createMesh(data);
        cache_[path] = mesh;
        return mesh;
    }
    catch (const std::exception& ex)
    {
        std::cerr << "MeshCache: failed to load '" << path << "': " << ex.what() << '\n';
        return nullptr;
    }
}

std::shared_ptr<Mesh> MeshCache::loadFromGenerator(const std::string& key,
                                                   const std::function<MeshData()>& generator)
{
    if (auto it = cache_.find(key); it != cache_.end())
    {
        if (auto existing = it->second.lock())
        {
            return existing;
        }
    }

    const MeshData data = generator ? generator() : MeshData{};
    auto mesh = createMesh(data);
    cache_[key] = mesh;
    return mesh;
}

void MeshCache::clear()
{
    cache_.clear();
}

std::shared_ptr<Mesh> MeshCache::createMesh(const MeshData& data)
{
    if (api_ == nullptr)
    {
        throw std::runtime_error("MeshCache has no associated RenderAPI.");
    }

    if (data.vertices.empty() || data.indices.empty())
    {
        throw std::runtime_error("MeshCache received empty mesh data.");
    }

    auto mesh = api_->createMesh();
    if (!mesh)
    {
        throw std::runtime_error("Failed to create mesh from RenderAPI.");
    }

    mesh->upload(data.vertices, data.indices);
    return std::shared_ptr<Mesh>(mesh.release());
}
} // namespace nre
